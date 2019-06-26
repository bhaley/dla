/// ============================================================================
// DLA.cc -- Diffusion Limited Aggregation simulator
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <exception>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "Params.h"
#include "Cluster.h"
#include "Property.h"
#include "Config.h"  // HAVE_MPI, HAVE_OPENMP, DLA_PROGRAM, DLA_VERSION

#ifdef HAVE_MPI
#include <mpi.h>
#endif
#ifdef HAVE_OPENMP
#include <omp.h>
#endif

namespace po = boost::program_options;

using std::string;
using std::exception;
using std::cerr;
using std::cout;
using std::ostream;
using std::ofstream;

typedef DLA::Property<double> ScalarProp;
typedef DLA::Property<const std::vector<double>&> VectorProp;

typedef boost::shared_ptr<DLA::PropertyInterface> PIptr;
typedef std::list<PIptr> PIptr_list;

// File scope
static DLA::Timer status_timer;

// ============================================================================
// Write version and configuration to os
// ============================================================================
static void
write_version(ostream& os)
{
   os << DLA_PROGRAM << " version " << DLA_VERSION;
   os << "   (MPI: " <<
#ifdef HAVE_MPI
         "enabled";
#else
         "disabled";
#endif
   os << ")   (OpenMP: " << 
#ifdef HAVE_OPENMP
         "enabled";
#else
         "disabled";
#endif
   os << ")\n";
}

// ============================================================================
// Write status bar and completion percentage (pct) to stdout
// ============================================================================
static void
write_status(int pct)
{
#ifdef RAPPTURE_STATUS
   cout << "=RAPPTURE-PROGRESS=>" << pct << "\n";
#else
   const int barlen = 30;
   int max = static_cast<int>(pct*barlen*0.01);

   cout << '|';
   for (int i = 0; i < barlen; i++)
      cout << ((i < max) ? '=' : ' ');
   cout << "| " << pct << "% (" << status_timer.time() << " s)   "
        << ((pct == 100) ? "\n\n" : "\r");
   cout.flush();
#endif
}

// ============================================================================
// Parse command line and set infile; exit(0) on -v or -h and exit(1) on flag 
// error
// ============================================================================
static void
parse_command_line(int argc, char *argv[], string& infile, int mpi_rank)
{
   po::options_description flags("Usage: dla <flag> | <infile>\nFlags");
   po::options_description full;
   po::positional_options_description hidden;
   po::variables_map arg_map;

   flags.add_options()
      ("version,v",  "Show version info and exit")
      ("help,h",     "Show help message and exit");
   full.add(flags).add_options()
      ("infile",     "(Hidden) input file");
   hidden.add("infile", -1); // last command line arg
   try
   {
      po::store(po::command_line_parser(argc, argv).options(full).
                     positional(hidden).run(), arg_map);
      po::notify(arg_map);
   }
   catch (exception& e)
   {
      if (0 == mpi_rank)
         cerr << flags << "\n" << e.what() << "\n";
      exit(1);
   }
   if (arg_map.count("version"))
   {
      if (0 == mpi_rank)
         write_version(cout);
      exit(0);
   }
   if (arg_map.count("help"))
   {
      if (0 == mpi_rank)
         cout << flags;
      exit(0);
   }
   if (!arg_map.count("infile"))
   {
      if (0 == mpi_rank)
         cerr << flags << "\nMissing input file\n";
      exit(1);
   }
   infile = arg_map["infile"].as<string>();
}

// ============================================================================
// Main simulation function
// ============================================================================
static void
grow_cluster(const DLA::Params& params, int cid, PIptr_list& plist, bool status)
{
   DLA::Cluster c(params.cluster_particles);
   DLA::RNG rng(params.rng_seed + static_cast<uint32_t>(cid));
   DLA::Position p0(0.0, 0.0, 0.0, 0);  // seed position
   DLA::Position cp;
   DLA::Position p;
   int steps;
   int n;
   const double dmin = 2.0*params.particle_radius;
   double dmax;
   double d;
   bool captured;
   DLA::Position diff;
   string path;
   ofstream of;

   if (params.density_bin > 0.0)
      c.density_setup(params.get_max_distance(), params.density_bin);

   // Add seed particle
   c.add_position(p0);

   while (c.get_size() < params.cluster_particles)
   {  // Start a new walker
      c.add_walker();
      p = p0;
      p.index = c.get_size();
      params.medium->displace(p, params.start_distance+c.get_radius(), rng);
      dmax = p.distance(p0);

      // Diffusion
      captured = false;
      steps = 0;
      while (!captured)
      {
         params.medium->step(p, params.step_size, rng);
         steps++;
         if (p.distance(p0) > dmax)
            break;  // walker escaped
         if (c.find_nearest(p, cp, dmin))
         {
            d = p.distance(cp);
            if ((d < params.capture_distance) && 
                (rng.yield() < params.stickiness))
            {
               captured = true;
               c.add_branch(cp.index, p.index);
               if (d < dmin)
               {  // Overlapping particles
                  diff = p - cp;
                  d = dmin/diff.Norm();
                  p.x = cp.x + d*diff.x;
                  p.y = cp.y + d*diff.y;
                  p.z = cp.z + d*diff.z;
               }
            }
         }
      }

      if (captured)
      {
         c.add_position(p);
         c.add_steps(steps);
         n = c.get_size();
         if ((params.growth_step > 0) && (0 == n % params.growth_step))
            c.update_growth();
         if ((params.xyz_step > 0) && (0 == n % params.xyz_step))
         {
            path = boost::str(boost::format("cluster_%04d_step_%06d.xyz") % cid
                   % c.get_size());
            of.open(path.c_str());
            if (of)
            {
               c.write_xyz(of);
               of.close();
            }
         }
         if ((status) && (params.status_step > 0) && 
             (0 == n % params.status_step))
         {
            write_status(static_cast<int>(100.0*n/params.cluster_particles));
         }
      }
   }
   if (params.density_bin > 0.0)
      c.finalize_density(params.medium);

#pragma omp critical (collect_stats)
   {
      PIptr_list::iterator pit = plist.begin();

      while (pit != plist.end())
      {
         (*pit)->collect(c);
         pit++;
      }
   }
}

// ============================================================================
// Ensure that MPI::Finalize() is called
// ============================================================================
void
dla_exit(void)
{
#ifdef HAVE_MPI
   MPI::Finalize();
#endif
}

// ============================================================================
// Top level -- return 0 on success, exit(1) on error
// ============================================================================
int
main(int argc, char *argv[])
{
   int mpi_rank = 0;
   int mpi_size = 1;
   int proc_clusters;
   string infile;
   std::ifstream fi;
   std::ostringstream errmsg;
   ostream log_stream(cout.rdbuf());     // log to stdout by default
   ostream status_stream(cout.rdbuf());  // status to stdout by default
   ofstream logf;
   DLA::Params params;

   // ----------------------------------------------------------
   // Initialize
   // ----------------------------------------------------------
#ifdef HAVE_MPI
   MPI::Init(argc, argv);
   mpi_rank = MPI::COMM_WORLD.Get_rank();
   mpi_size = MPI::COMM_WORLD.Get_size();
   MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);
#endif
   atexit(dla_exit);  // ensure that MPI::Finalize() is called
   status_timer.start();

   // ----------------------------------------------------------
   // Input and division of Clusters across nodes, threads
   // ----------------------------------------------------------
   int status;
   parse_command_line(argc, argv, infile, mpi_rank);
   fi.open(infile.c_str());
   if (!fi)
   {
      if (0 == mpi_rank)
         cerr << "Unable to open input file " << infile << "\n";
      exit(1);
   }
   status = params.parse(fi, errmsg);
   fi.close();
   if (1 == status)
   {
      if (0 == mpi_rank)
      {
         cerr << "Parse error in input file " << infile << ": " << errmsg.str() 
              << "\n";
      }
      exit(1);
   }
   proc_clusters = params.total_clusters/mpi_size;
   if (0 != params.total_clusters % mpi_size)
   {
      if (0 == mpi_rank)
      {
         cerr << "ERROR: " << params.total_clusters << " is not evenly ";
         cerr << "divisible across " << mpi_size << " nodes\n";
      }
      exit(1);
   }
   if (params.process_threads > proc_clusters)
      params.process_threads = proc_clusters;
   // TODO more sanity checks for params...

   // ----------------------------------------------------------
   // Log
   // ----------------------------------------------------------
   if ((0 == mpi_rank) && (params.log_file.size() > 0))
   {
      logf.open(params.log_file.c_str());
      if (!logf)
         cerr << "Unable to open log file " << params.log_file << "\n";
      else
         log_stream.rdbuf(logf.rdbuf());
   }
   if (0 == mpi_rank)
   {
      int cpt = proc_clusters/params.process_threads;

      log_stream << "=======================================";
      log_stream << "=======================================\n";
      write_version(log_stream);
      log_stream << "=======================================";
      log_stream << "=======================================\n\n";
      log_stream << params;
      log_stream << "\n";
      log_stream << "Growing " << params.total_clusters << " "
                 << ((params.total_clusters > 1) ? "clusters" : "cluster")
                 << " on " << mpi_size << ((mpi_size > 1) ? " nodes" : " node")
                 << ": " << proc_clusters << " on each node\n";
      log_stream << "   " << params.process_threads << " "
                 << ((params.process_threads > 1) ? "threads" : "thread")
                 << " per process: " << cpt << " "
                 << ((cpt > 1) ? "clusters" : "cluster") << " per thread\n\n";
   }

   // ----------------------------------------------------------
   // Properties of interest
   // ----------------------------------------------------------
   PIptr_list plist;

   plist.push_back(PIptr(new ScalarProp("radius", 1, 
                                    &DLA::Cluster::get_radius, 1.0)));
   plist.push_back(PIptr(new ScalarProp("radius of gyration", 1, 
                                    &DLA::Cluster::get_radius_gyration, 1.0)));
   plist.push_back(PIptr(new ScalarProp("capture fraction", 1, 
                                    &DLA::Cluster::get_capture_fraction, 1.0)));
   plist.push_back(PIptr(new ScalarProp("free steps", 1, 
                                    &DLA::Cluster::get_mean_free_steps, 1.0)));
   plist.push_back(PIptr(new ScalarProp("growth time", 1, 
                                    &DLA::Cluster::get_growth_time, 1.0)));
   if (params.growth_step > 0)
   {
      int nrad = params.cluster_particles/params.growth_step;
      double step = static_cast<double>(params.growth_step);

      plist.push_back(PIptr(new VectorProp("radius_growth", nrad, 
                                       &DLA::Cluster::get_radius_growth,step)));
                                       // use default column format
      plist.push_back(PIptr(new VectorProp("radius_gyration_growth", nrad, 
                              &DLA::Cluster::get_radius_gyration_growth,step)));
                                       // use default column format
   }
   if (params.density_bin > 0.0)
   {
      int ndens =static_cast<int>(params.get_max_distance()/params.density_bin);

      plist.push_back(PIptr(new VectorProp("density_correlation", ndens, 
                                       &DLA::Cluster::get_density_corr, 
                                       params.density_bin, 
                                       "%8.2f%15.8f%15.8f\n")));
   }

   // ----------------------------------------------------------
   // Grow Clusters
   // ----------------------------------------------------------
   int n = 0;
   int cid;
#pragma omp parallel private(cid) num_threads(params.process_threads)
   {  // Begin parallel region; n is a shared counter for this process
      while (n < proc_clusters)
      {
#pragma omp critical (unique_index)
         {  // Unique index for each Cluster -- used in RNG seed
            cid = mpi_rank*proc_clusters + n;
            n++;
         }
         try
         {
            grow_cluster(params, cid, plist, ((0 == mpi_rank) && (0 == cid)));
         }
         catch (exception& e)
         {  // Write message on any node
            cerr << "ERROR on node " << mpi_rank << "during cluster growth: " 
                 << e.what() << "\n";
            exit(1);
         }
      }
   }  // End parallel region

   // ----------------------------------------------------------
   // Output
   // ----------------------------------------------------------
   try 
   {
      PIptr_list::iterator pit = plist.begin();
      string path;
      ofstream fo;

      while (pit != plist.end())
      {
         if ((*pit)->is_scalar())
            (*pit)->report(log_stream, mpi_rank, mpi_size);
         else
         {
            path = (*pit)->get_name() + ".dat";
            fo.open(path.c_str());
            if (!fo)
               log_stream << "Unable to open output file " << path << "\n";
            else
            {
               (*pit)->report(fo, mpi_rank, mpi_size);
               fo.close();
               log_stream << "Wrote " << path << "\n";
            }
         }
         pit++;
      }
   }
#ifdef HAVE_MPI
   catch (MPI::Exception& empi)
   {  // MPI error: write message on any node
      cerr << "MPI ERROR " << empi.Get_error_code() << " on node "
           << mpi_rank << ": " << empi.Get_error_string() << "\n";
      exit(1);
   }
#endif
   catch (exception& e)
   {  // Write message on any node
      cerr << "ERROR while writing output: " << e.what() << "\n";
      exit(1);
   }

   // ----------------------------------------------------------
   // Finish
   // ----------------------------------------------------------
   if (0 == mpi_rank)
   {
      if (log_stream.rdbuf() != cout.rdbuf())
         logf.close();
   }
   return 0;
}
