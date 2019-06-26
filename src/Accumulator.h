// ============================================================================
// Accumulator.h -- DLA::Accumulator class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_ACCUMULATOR_H
#define DLA_ACCUMULATOR_H

#include <vector>
#include <ostream>
#include <string>
#include "Config.h"  // HAVE_MPI

namespace DLA {

// Statistical accumulator: calculate mean and (biased) standard deviation for 
// multiple samples
class Accumulator
{
   public:

   // Constructor
   Accumulator(int size, double xscale = 1.0); 
            
   // Synthesized copy constructor, copy assignment, and destructor

   // Accumulate scalar values; throw out_of_range exception 
   // if sample_size != 1
   void accumulate(double v, double wt);

   // Accumulate vector values; throw out_of_range exception 
   // if sample_size != v.size()
   void accumulate(const std::vector<double>& v, double wt);

   // Calculate mean and stddev; may require MPI communication with other nodes;
   // throw a logic_error exception if any weight is 0.0 or negative
   void finalize(int mpi_rank, int mpi_size);

   // Return the sample size
   int get_sample_size(void) const
   {
      return sample_size;
   }

   // Report scalar mean and stddev to os
   void report_scalar(std::ostream& os) const
   {
      os << mean[0] << " (stddev: " << stddev[0] << ")\n";
   }

   // Report vector mean and stddev to os in column format
   void report_vector(std::ostream& os, const std::string& column_format) const;

   private:

   int sample_size;
   std::vector<double> weight;
   std::vector<double> sum;
   std::vector<double> sum_sq;
#ifdef HAVE_MPI
   std::vector<double> recv_weight;
   std::vector<double> recv_sum;
   std::vector<double> recv_sum_sq;
#endif
   std::vector<double> mean;
   std::vector<double> stddev;
   double xscale;
};
   
}  // end DLA

#endif  // DLA_ACCUMULATOR_H
