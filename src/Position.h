// ============================================================================
// Position.h -- DLA::Position class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_POSITION_H
#define DLA_POSITION_H

#include <vector>
#include <cmath>

namespace DLA {

// 3D Cartesian position
class Position
{
   public:

   // Constructor
   Position(double x = 0.0, double y = 0.0, double z = 0.0, int n = -1): 
      x(x), y(y), z(z), index(n)
   {}

   // Synthesized copy constructor, copy assignment, and destructor

   // Return the distance from this Position to other
   double distance(const Position& other) const 
   {
      double dx = x - other.x;
      double dy = y - other.y;
      double dz = z - other.z;

      return sqrt(dx*dx + dy*dy + dz*dz);
   }

   // Return the distance from the origin (used by NearTree)
   double Norm(void) const 
   {
      return sqrt(x*x + y*y + z*z);
   }

   // Return a new Position, this Position relative to other (used by NearTree)
   Position operator-(const Position& other) const
   {
      return Position(x-other.x, y-other.y, z-other.z);
   }

   // Return the particle index
   int get_index(void) const
   {
      return index;
   }

   // Cartesian coordaintes
   double x;
   double y;
   double z;

   // Particle index in NearTree object store
   int index;
}; 

}  // end DLA

#endif  // DLA_POSITION_H
