// ============================================================================
// Tests.cc -- OpenDLA test suite
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OpenDLA unit tests

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/function.hpp>
#include <list>
#include "Position.h"  // <cmath>
#include "RNG.h"
#include "Lattice2D.h"
#include "Lattice3D.h"
#include "Fluid2D.h"
#include "Fluid3D.h"
#include "Params.h" // <string>, <sstream>
#include "Cluster.h"
#include "Accumulator.h"
#include "Property.h" // PropertyInterface

using std::string;
using std::vector;
using std::ostringstream;

static const double strict_tol = 1.0e-9; // %
static const uint32_t rng_seed = 192837465;

// ============================================================================
// Position
// ============================================================================

BOOST_AUTO_TEST_CASE(test_Position)
{
   const DLA::Position p1(0.0, 0.0, 0.0);
   DLA::Position p2(1.0, -2.0, 3.0);
   DLA::Position p3 = p1 - p2;
   const double sqrt14 = sqrt(14.0);

   BOOST_CHECK_CLOSE(sqrt14, p1.distance(p2), strict_tol);
   BOOST_CHECK_CLOSE(sqrt14, p2.distance(p1), strict_tol);
   BOOST_CHECK_CLOSE(sqrt14, p2.Norm(), strict_tol);
   BOOST_CHECK_CLOSE(-1.0, p3.x, strict_tol);
   BOOST_CHECK_CLOSE( 2.0, p3.y, strict_tol);
   BOOST_CHECK_CLOSE(-3.0, p3.z, strict_tol);
}

// ============================================================================
// RNG
// ============================================================================

BOOST_AUTO_TEST_CASE(test_RNG)
{
   DLA::RNG rng(rng_seed);
   const int nsamples = 100000000;
   const int nbins = 100;
   int count[nbins];
   int i;
   double target;

   // Reproducible sequence
   BOOST_CHECK_CLOSE(0.869853707263, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.213025633246, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.825101681286, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.791415012674, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.234758293955, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.200296355411, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.303612547461, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.197595334845, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.533934774809, rng.yield(), strict_tol);
   BOOST_CHECK_CLOSE(0.460835322971, rng.yield(), strict_tol);
   
   // Uniform distribution
   for (i = 0; i < nbins; i++)
      count[i] = 0;
   for (i = 0; i < nsamples; i++)
      count[static_cast<int>(rng.yield()*nbins)]++;
   target = static_cast<double>(nsamples/nbins);
   for (i = 0; i < nbins; i++)
   {  // count[i] should be within 0.5% of target
      BOOST_CHECK_CLOSE(static_cast<double>(count[i]), target, 0.5);
   }
}

// ============================================================================
// Medium
// ============================================================================

BOOST_AUTO_TEST_CASE(test_Medium)
{
   DLA::Medium *m;
   const double a = 0.72;
   const double g = 25.0*a;
   double d;
   DLA::RNG rng(rng_seed);
   DLA::Position p(0.0,0.0,0.0);
   DLA::Position pinit;

   m = new DLA::Lattice2D(a);
   pinit = p;
   m->displace(p, g, rng);
   d = p.distance(pinit);
   BOOST_CHECK_EQUAL(true, ((d > g) && (d < g+M_SQRT2*a)));
   pinit = p;
   m->step(p, g, rng);
   d = p.distance(pinit);
   BOOST_CHECK_CLOSE(a, d, strict_tol);
   BOOST_CHECK_CLOSE(M_PI, m->volume(1.0), strict_tol);
   BOOST_CHECK_CLOSE(M_PI, m->surface_area(0.5), strict_tol);
   delete m;

   m = new DLA::Lattice3D(a);
   pinit = p;
   m->displace(p, g, rng);
   d = p.distance(pinit);
   BOOST_CHECK_EQUAL(true, ((d > g) && (d < g+sqrt(3.0)*a)));
   pinit = p;
   m->step(p, g, rng);
   d = p.distance(pinit);
   BOOST_CHECK_CLOSE(a, d, strict_tol);
   BOOST_CHECK_CLOSE(M_4PI_3, m->volume(1.0), strict_tol);
   BOOST_CHECK_CLOSE(M_4PI, m->surface_area(1.0), strict_tol);
   delete m;

   m = new DLA::Fluid2D();
   pinit = p;
   m->displace(p, g, rng);
   d = p.distance(pinit);
   BOOST_CHECK_CLOSE(g, d, strict_tol);
   pinit = p;
   m->step(p, g, rng);
   d = p.distance(pinit);
   BOOST_CHECK_CLOSE(g, d, strict_tol);
   BOOST_CHECK_CLOSE(M_PI, m->volume(1.0), strict_tol);
   BOOST_CHECK_CLOSE(M_PI, m->surface_area(0.5), strict_tol);
   delete m;

   m = new DLA::Fluid3D();
   pinit = p;
   m->displace(p, g, rng);
   d = p.distance(pinit);
   BOOST_CHECK_CLOSE(g, d, strict_tol);
   pinit = p;
   m->step(p, g, rng);
   d = p.distance(pinit);
   BOOST_CHECK_CLOSE(g, d, strict_tol);
   BOOST_CHECK_CLOSE(M_4PI_3, m->volume(1.0), strict_tol);
   BOOST_CHECK_CLOSE(M_4PI, m->surface_area(1.0), strict_tol);
   delete m;

   // Constants
   BOOST_CHECK_CLOSE(M_4PI_3, 4.0*M_PI/3.0, strict_tol);
   BOOST_CHECK_CLOSE(M_2PI,   2.0*M_PI, strict_tol);
   BOOST_CHECK_CLOSE(M_4PI,   4.0*M_PI, strict_tol);
}

// ============================================================================
// Params
// ============================================================================

BOOST_AUTO_TEST_CASE(test_Params)
{
   const string input = 
      "# comment\n"
      "total_clusters      = 4           # all processes, all threads\n"
      "process_threads     = 2\n"
      "cluster_particles   = 3e3\n"
      "medium              = fluid_3D\n"
      "start_distance      = 25.0\n"
      "step_size           = 1.0\n"
      "particle_radius     = 0.5\n"
      "capture_distance    = 1.01\n"
      "stickiness          = 1.0\n"
      "growth_step         = 30\n"
      "xyz_step            = 3e3\n"
      "status_step         = 15\n"
      "density_bin         = 2\n"
      "rng_seed            = 12345\n" 
      "log_file            = dla.log";
   const string output = 
      "Total clusters:    4\n"
      "Process threads:   2\n"
      "Cluster particles: 3000\n"
      "Medium:            fluid_3D\n"
      "Start distance:    25\n"
      "Step size:         1\n"
      "Particle radius:   0.5\n"
      "Capture distance:  1.01\n"
      "Stickiness:        1\n"
      "Growth step:       30\n"
      "XYZ step:          3000\n"
      "Status step:       15\n"
      "Density bin size:  2\n"
      "RNG seed:          12345\n";
   std::istringstream is(input);
   ostringstream os;
   DLA::Params p;

   BOOST_CHECK_EQUAL(0, p.parse(is, os));
   os << p;
   BOOST_CHECK_EQUAL(output, os.str());
}

// ============================================================================
// Cluster 
// ============================================================================

BOOST_AUTO_TEST_CASE(test_Cluster)
{
   DLA::Cluster c(5);
   DLA::Position p;
   DLA::Position pnear;
   const double sqrt8 = sqrt(8.0);
   const double sqrt5 = sqrt(5.0);
   //         4
   //         2
   //     0 1 
   //   3
   const string xyz_str = 
      "5\n"
      "Cluster\n"
      "    1           0.00           0.00           0.00    1\n"
      "    2           1.00           0.00           0.00    0\n"
      "    3           2.00           1.00           0.00    0\n"
      "    4          -1.00          -1.00           0.00    0\n"
      "    5           2.00           2.00           0.00    0\n";
   ostringstream os;
   DLA::MediumPtr m(new DLA::Lattice2D(1.0));

   BOOST_CHECK_EQUAL(0, c.get_size());
   BOOST_CHECK_CLOSE(0.0, c.get_radius(), strict_tol);

   c.density_setup(5.0, 1.0);

   // 0
   p.index = 0;
   c.add_position(p);
   c.update_growth();

   // 1
   p.index = 1;
   p.x = 1.0;
   c.find_nearest(p, pnear, 1.5);
   c.add_position(p);
   c.update_growth();
   c.add_branch(pnear.index, p.index);

   // 2
   p.index = 2;
   p.x = 2.0;
   p.y = 1.0;
   c.find_nearest(p, pnear, 1.5);
   c.add_position(p);
   c.update_growth();
   c.add_branch(pnear.index, p.index);

   // 3
   p.index = 3;
   p.x = -1.0;
   p.y = -1.0;
   c.find_nearest(p, pnear, 1.5);
   c.add_position(p);
   c.update_growth();
   c.add_branch(pnear.index, p.index);

   // 4
   p.index = 4;
   p.x = 2.0;
   p.y = 2.0;
   c.find_nearest(p, pnear, 1.5);
   c.add_position(p);
   c.update_growth();
   c.add_branch(pnear.index, p.index);

   for (int i = 0; i < 12; i++)
   {
      c.add_walker();
      c.add_steps(i);
   }

   BOOST_CHECK_EQUAL(5, c.get_size());
   BOOST_CHECK_CLOSE(sqrt8, c.get_radius(), strict_tol);
   BOOST_CHECK_CLOSE(sqrt8, c.get_radius(), strict_tol);
   BOOST_CHECK_CLOSE(0.416666666666666, c.get_capture_fraction(), strict_tol);
   BOOST_CHECK_CLOSE(13.2, c.get_mean_free_steps(), strict_tol);

   c.write_xyz(os);
   BOOST_CHECK_EQUAL(xyz_str, os.str());

   const vector<double>& rg = c.get_radius_growth();
   BOOST_CHECK_CLOSE(0.0, rg[0], strict_tol);
   BOOST_CHECK_CLOSE(1.0, rg[1], strict_tol);
   BOOST_CHECK_CLOSE(sqrt5, rg[2], strict_tol);
   BOOST_CHECK_CLOSE(sqrt5, rg[3], strict_tol);
   BOOST_CHECK_CLOSE(sqrt8, rg[4], strict_tol);

   p.x = 3.0;
   p.y = 1.0;
   BOOST_CHECK_EQUAL(true, c.find_nearest(p, pnear, 1.5));
   BOOST_CHECK_CLOSE(1.0, p.distance(pnear), strict_tol);

   p.x =  2.0;
   p.y = -1.0;
   BOOST_CHECK_EQUAL(true, c.find_nearest(p, pnear, 1.5));
   BOOST_CHECK_CLOSE(M_SQRT2, p.distance(pnear), strict_tol);

   p.x = -1.0;
   p.y =  1.0;
   BOOST_CHECK_EQUAL(true, c.find_nearest(p, pnear, 1.5));
   BOOST_CHECK_CLOSE(M_SQRT2, p.distance(pnear), strict_tol);

   p.x =  0.0;
   p.y = -2.0;
   BOOST_CHECK_EQUAL(true, c.find_nearest(p, pnear, 1.5));
   BOOST_CHECK_CLOSE(M_SQRT2, p.distance(pnear), strict_tol);

   c.finalize_density(m);
   const vector<double>& rrds = c.get_density_corr();
   BOOST_CHECK_CLOSE(0.0, rrds[0], strict_tol); // 0 
   BOOST_CHECK_CLOSE(0.12732395447352, rrds[1], strict_tol); // 4 
   BOOST_CHECK_CLOSE(0.06366197723676, rrds[2], strict_tol); // 4
   BOOST_CHECK_CLOSE(0.01061032953946, rrds[3], strict_tol); // 1
   BOOST_CHECK_CLOSE(0.00795774715459, rrds[4], strict_tol); // 1
}

// ============================================================================
// Accumulator
// ============================================================================

BOOST_AUTO_TEST_CASE(test_Accumulator)
{
   DLA::Accumulator a(1);
   ostringstream os;

   a.accumulate(373, 1.0);
   a.accumulate(358, 1.0);
   a.accumulate(394, 1.0);
   a.accumulate(378, 1.0);
   a.accumulate(379, 1.0);
   a.accumulate(367, 1.0);
   a.accumulate(364, 2.0);
   a.accumulate(350, 1.0);
   a.accumulate(363, 1.0);
   a.accumulate(392, 1.0);
   a.accumulate(368, 1.0);
   a.accumulate(359, 1.0);
   a.accumulate(375, 1.0);
   a.accumulate(399, 1.0);
   a.accumulate(365, 1.0);
   a.accumulate(379, 1.0);
   a.accumulate(357, 1.0);
   a.accumulate(380, 1.0);
   a.accumulate(400, 1.0);
   a.finalize(0, 1);
   a.report_scalar(os);
   BOOST_CHECK_EQUAL("373.2 (stddev: 14.052)\n", os.str());
}

// ============================================================================
// Property
// ============================================================================

BOOST_AUTO_TEST_CASE(test_Property)
{
   DLA::Cluster c(5);
   DLA::Position p;

   c.density_setup(5.0, 1.0);
   c.add_position(p);
   c.update_growth();
   p.x = 1.0;
   c.add_position(p);
   c.update_growth();
   p.x = 2.0;
   p.y = 1.0;
   c.add_position(p);
   c.update_growth();
   p.x = -1.0;
   p.y = -1.0;
   c.add_position(p);
   c.update_growth();
   p.x = 2.0;
   p.y = 2.0;
   c.add_position(p);
   c.update_growth();

   typedef DLA::Property<double> ScalarProp;
   typedef DLA::Property<const std::vector<double>&> VectorProp;

   typedef boost::shared_ptr<DLA::PropertyInterface> PIptr;
   typedef std::list<PIptr> PIptr_list;

   PIptr_list plist;

   plist.push_back(PIptr(new ScalarProp("radius", 1, &DLA::Cluster::get_radius,
                                        1.0)));
   plist.push_back(PIptr(new ScalarProp("radius of gyration", 1, 
                                     &DLA::Cluster::get_radius_gyration, 1.0)));
   plist.push_back(PIptr(new VectorProp("radius_growth", 5, 
                                        &DLA::Cluster::get_radius_growth,1.2)));
   plist.push_back(PIptr(new VectorProp("radius_gyr_growth", 5, 
                                      &DLA::Cluster::get_radius_gyration_growth,
                                      1.1)));

   const string rad_str = "Mean radius: 2.82843 (stddev: 0)\n";
   const string rgyr_str = "Mean radius of gyration: 1.09545 (stddev: 0)\n";
   ostringstream os;

   PIptr_list::iterator pit = plist.begin();
   (*pit)->collect(c);
   (*pit)->report(os, 0, 1);
   BOOST_CHECK_EQUAL(rad_str, os.str());
   pit++;
   (*pit)->collect(c);
   os.str("");
   (*pit)->report(os, 0, 1);
   BOOST_CHECK_EQUAL(rgyr_str, os.str());

   const string rg_str("#    x           mean          stddev\n    1.20           0.00           0.00\n    2.40           1.00           0.00\n    3.60           2.24           0.00\n    4.80           2.24           0.00\n    6.00           2.83           0.00\n");
   const string rgg_str("#    x           mean          stddev\n    1.10           0.00           0.00\n    2.20           0.35           0.00\n    3.30           0.67           0.00\n    4.40           0.94           0.00\n    5.50           1.10           0.00\n");

   pit++;
   (*pit)->collect(c);
   os.str("");
   (*pit)->report(os, 0, 1);
   BOOST_CHECK_EQUAL(rg_str, os.str());

   pit++;
   (*pit)->collect(c);
   os.str("");
   (*pit)->report(os, 0, 1);
   BOOST_CHECK_EQUAL(rgg_str, os.str());
}
