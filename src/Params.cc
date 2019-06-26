// ============================================================================
// Params.cc -- DLA::Params methods
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#include <boost/program_options.hpp>
#include <stdexcept>
#include <ctime>
#include "Params.h" // <ostream>, <istream>, Medium.h
#include "Lattice2D.h"
#include "Lattice3D.h"
#include "Fluid2D.h"
#include "Fluid3D.h"

namespace po = boost::program_options;

using std::ostream;
using std::string;

// ============================================================================
// Constructor
// ============================================================================
DLA::Params::Params() :
   total_clusters(0),
   process_threads(0),
   cluster_particles(0),
   growth_step(0),
   xyz_step(0),
   status_step(0),
   rng_seed(0),
   medium(DLA::MediumPtr()),
   start_distance(0.0),
   step_size(0.0),
   particle_radius(0.0),
   capture_distance(0.0),
   stickiness(0.0),
   density_bin(0.0),
   log_file(""),
   medium_name("") {}

// ============================================================================
// Parse input, populate Params; return 0 on success, 1 on error with error 
// message written to es 
// ============================================================================
int
DLA::Params::parse(std::istream& is, std::ostringstream& es)
{
   po::options_description config("Config");
   po::variables_map config_map;
   double tc, pt, cp, gs, xs, ss;

   rng_seed = static_cast<uint32_t>(time(NULL));
   config.add_options()
      ("total_clusters",     po::value<double>(&tc), "")
      ("process_threads",    po::value<double>(&pt), "")
      ("cluster_particles",  po::value<double>(&cp), "")
      ("growth_step",        po::value<double>(&gs), "")
      ("xyz_step",           po::value<double>(&xs), "")
      ("status_step",        po::value<double>(&ss), "")
      ("rng_seed",           po::value<uint32_t>(&rng_seed), "")
      ("start_distance",     po::value<double>(&start_distance), "")
      ("step_size",          po::value<double>(&step_size), "")
      ("particle_radius",    po::value<double>(&particle_radius), "")
      ("capture_distance",   po::value<double>(&capture_distance), "")
      ("stickiness",         po::value<double>(&stickiness), "")
      ("density_bin",        po::value<double>(&density_bin), "")
      ("log_file",           po::value<string>(&log_file), "")
      ("medium",             po::value<string>(&medium_name), "");
   try
   {
      po::store(po::parse_config_file(is, config), config_map);
      notify(config_map);
   }
   catch (std::exception& e)
   {
      es << "Parse error: " << e.what();
      return 1;
   }
   if (medium_name == "lattice_2D")
      medium.reset(new DLA::Lattice2D(step_size));
   else if (medium_name == "lattice_3D")
      medium.reset(new DLA::Lattice3D(step_size));
   else if (medium_name == "fluid_2D")
      medium.reset(new DLA::Fluid2D);
   else if (medium_name == "fluid_3D")
      medium.reset(new DLA::Fluid3D);
   else
   {
      es << "Unknown medium: " << medium_name;
      return 1;
   }
   total_clusters    = static_cast<int>(tc);
   process_threads   = static_cast<int>(pt);
   cluster_particles = static_cast<int>(cp);
   growth_step       = static_cast<int>(gs);
   xyz_step          = static_cast<int>(xs);
   status_step       = static_cast<int>(ss);
   return 0;
}

// ============================================================================
// Write simulation parameters to os
// ============================================================================
ostream& 
DLA::operator<<(ostream& os, const Params& p)
{
   os << "Total clusters:    " << p.total_clusters << "\n";
   os << "Process threads:   " << p.process_threads << "\n";
   os << "Cluster particles: " << p.cluster_particles << "\n";
   os << "Medium:            " << p.medium_name << "\n";
   os << "Start distance:    " << p.start_distance << "\n";
   os << "Step size:         " << p.step_size << "\n";
   os << "Particle radius:   " << p.particle_radius << "\n";
   os << "Capture distance:  " << p.capture_distance << "\n";
   os << "Stickiness:        " << p.stickiness << "\n";
   os << "Growth step:       " << p.growth_step << "\n";
   os << "XYZ step:          " << p.xyz_step << "\n";
   os << "Status step:       " << p.status_step << "\n";
   os << "Density bin size:  " << p.density_bin << "\n";
   os << "RNG seed:          " << p.rng_seed << "\n";
   return os;
}
