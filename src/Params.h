// ============================================================================
// Params.h -- DLA::Params class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_PARAMS_H
#define DLA_PARAMS_H

#include <boost/utility.hpp>
#include <sstream>
#include <string>
#include "Medium.h"

namespace DLA {

// Simulation parameters and input file parser
class Params : private boost::noncopyable
{
   public:

   // Constructor
   Params();

   // noncopyable prevents copy constructor and copy assignment

   // Destructor; shared_ptr ensures that medium is deleted
   ~Params() {}

   // Return the max possible separation between cluster particles
   double get_max_distance(void) const
   {
      return 2.0*particle_radius*static_cast<double>(cluster_particles);
   }

   // Parse input, populate Params; return 0 on success, 1 on error with 
   // error message written to es 
   int parse(std::istream& is, std::ostringstream& es);

   int total_clusters;
   int process_threads;
   int cluster_particles;
   int growth_step;
   int xyz_step;
   int status_step;
   uint32_t rng_seed;
   MediumPtr medium;
   double start_distance;
   double step_size;
   double particle_radius;
   double capture_distance;
   double stickiness;
   double density_bin;
   std::string log_file;
   std::string medium_name;
}; 

// Write simulation parameters to os
std::ostream& operator<<(std::ostream& os, const Params& p);

}  // end DLA

#endif  // DLA_PARAMS_H
