// ============================================================================
// Accumulator.cc -- DLA::Accumulator methods
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#include <boost/format.hpp>
#include <stdexcept>
#include <cmath>
#include "Accumulator.h"
#include "Config.h" // HAVE_MPI
#ifdef HAVE_MPI
#include <mpi.h>
#endif

using std::vector;
using std::out_of_range;

// ============================================================================
// Constructor
// ============================================================================
DLA::Accumulator::Accumulator(int size, double xscale) :
   sample_size(size), 
   weight(vector<double>(size)), 
   sum(vector<double>(size)),
   sum_sq(vector<double>(size)),
#ifdef HAVE_MPI
   recv_weight(vector<double>(size)), 
   recv_sum(vector<double>(size)),
   recv_sum_sq(vector<double>(size)),
#endif
   mean(vector<double>(size)), 
   stddev(vector<double>(size)), 
   xscale(xscale)
{
   for (int i = 0; i < sample_size; i++)
   {  // Explicit initialization
      sum[i] = 0.0;
      sum_sq[i] = 0.0;
      weight[i] = 0.0;
#ifdef HAVE_MPI
      recv_sum[i] = 0.0;
      recv_sum_sq[i] = 0.0;
      recv_weight[i] = 0.0;
#endif
   }
}

// ============================================================================
// Accumulate scalar values; throw out_of_range exception if sample_size != 1
// ============================================================================
void 
DLA::Accumulator::accumulate(double v, double wt)
{
   if (sample_size != 1)
      throw out_of_range("Scalar sample accumulated to non-scalar property");
   sum[0] += v*wt;
   sum_sq[0] += v*v*wt;
   weight[0] += wt;
}

// ============================================================================
// Accumulate vector values; throw out_of_range exception if 
// sample_size != v.size()
// ============================================================================
void 
DLA::Accumulator::accumulate(const std::vector<double>& v, double wt)
{
   if (sample_size != v.size())
      throw out_of_range("Vector sample size mismatch");
   for (int i = 0; i < sample_size; i++)
   {
      sum[i] += v[i]*wt;
      sum_sq[i] += v[i]*v[i]*wt;
      weight[i] += wt;
   }
}

// ============================================================================
// Calculate mean and stddev; may require MPI communication with other nodes;
// throw a logic_error exception if any weight is 0.0 or negative
// ============================================================================
void 
DLA::Accumulator::finalize(int mpi_rank, int mpi_size)
{
   int i;
   double z;
#ifdef HAVE_MPI
   if (mpi_size > 1)
   {
      MPI::COMM_WORLD.Barrier();
      MPI::COMM_WORLD.Reduce(&sum[0], &recv_sum[0], sample_size, MPI_DOUBLE, 
                             MPI_SUM, 0);
      MPI::COMM_WORLD.Reduce(&sum_sq[0], &recv_sum_sq[0], sample_size,
                             MPI_DOUBLE, MPI_SUM, 0);
      MPI::COMM_WORLD.Reduce(&weight[0], &recv_weight[0], sample_size,
                             MPI_DOUBLE, MPI_SUM, 0);
      if (0 == mpi_rank)
      {
         for (i = 0; i < sample_size; i++)
         {
            sum[i] = recv_sum[i];
            sum_sq[i] = recv_sum_sq[i];
            weight[i] = recv_weight[i];
         }
      }
   }
#endif
   for (i = 0; i < sample_size; i++)
   {
      if (!(weight[i] > 0.0))
         throw std::logic_error("Invalid weight for property");
      mean[i] = sum[i]/weight[i];
      // biased standard deviation using second moment
      stddev[i] = sqrt(sum_sq[i]/weight[i] - mean[i]*mean[i]);
   }
}

// ============================================================================
// Report mean and stddev to os
// ============================================================================
void 
DLA::Accumulator::report_vector(std::ostream& os, 
                                const std::string& column_format) const
{
   double x;

   for (int i = 0; i < sample_size; i++)
   {
      x = static_cast<double>(i+1)*xscale; 
      os << boost::format(column_format.c_str()) % x % mean[i] % stddev[i];
   }
}
