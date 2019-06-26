// ============================================================================
// PropertyInterface.h -- DLA::PropertyInterface class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_PROPERTY_INTERFACE_H
#define DLA_PROPERTY_INTERFACE_H

#include <string>
#include <ostream>
#include "Cluster.h"

namespace DLA {

// Abstract interface for Property class, so we can create Property objects
// that collect different types (scalar double and std::vector<double>&) from
// a Cluster and pack them into a container
class PropertyInterface
{
   public:

   // Constructor
   PropertyInterface() {}
            
   // Synthesized copy constructor, copy assignment

   // Destructor
   virtual ~PropertyInterface() {}

   // Collect and record the property of interest from the Cluster c
   virtual void collect(const Cluster& c) = 0;

   // Return true if the property is scalar, else return false
   virtual bool is_scalar(void) const = 0;

   // Return the property name
   virtual const std::string& get_name(void) const = 0;

   // Calculate final mean and stddev; report stats to os
   virtual void report(std::ostream& os, int mpi_rank, int mpi_size) = 0;
};
   
}  // end DLA

#endif  // DLA_PROPERTY_INTERFACE_H
