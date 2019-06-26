// ============================================================================
// Property.h -- DLA::Property class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_PROPERTY_H
#define DLA_PROPERTY_H

#include <boost/function.hpp>
#include "Accumulator.h"       // <ostream>
#include "PropertyInterface.h" // <string>, Cluster.h

namespace DLA {

// Associate a physical property of interest with a boost::function to return
// the property from a Cluster and an Accumulator to keep statistics
template <typename T>
class Property : public PropertyInterface
{
   public:

   // Constructor
   Property(const std::string& pname, int size, T (Cluster::*f)(void) const,
            double xscale, const std::string& format = "%8.2f%15.2f%15.2f\n") : 
      PropertyInterface(), name(pname), acc(size, xscale), op(f), 
      column_format(format)
   {}
            
   // Synthesized copy constructor, copy assignment, and destructor

   // Collect and record the property of interest from the Cluster c
   void collect(const Cluster& c) 
   {
      acc.accumulate(op(c), 1.0);
   }

   // Return true if the property is scalar, else return false
   bool is_scalar(void) const
   {
      return (1 == acc.get_sample_size());
   }

   // Return the property name
   const std::string& get_name(void) const
   {
      return name;
   }

   // Calculate final mean and stddev; report stats to os
   void report(std::ostream& os, int mpi_rank, int mpi_size)
   {
      acc.finalize(mpi_rank, mpi_size);
      if (0 == mpi_rank)
      {
         if (1 == acc.get_sample_size())
         {
            os << "Mean " << name << ": "; 
            acc.report_scalar(os);
         }
         else
         {
            os << "#    x           mean          stddev\n";
            acc.report_vector(os, column_format);
         }
      }
   }

   private:

   std::string name;
   Accumulator acc;
   boost::function<T (const Cluster&)> op; 
   std::string column_format;
};
   
}  // end DLA

#endif  // DLA_PROPERTY_H
