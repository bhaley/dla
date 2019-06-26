// ============================================================================
// Fluid3D.h -- DLA::Fluid3D class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_FLUID_3D_H
#define DLA_FLUID_3D_H

#include "Medium.h"  // <cmath>

namespace DLA {

// Medium for 3D diffusion; (x,y,z) can take any values
class Fluid3D : public Medium
{
   public:

   // Constructor
   Fluid3D() : Medium(1.0) {}

   // noncopyable prevents copy constructor and copy assignment

   // Destructor
   ~Fluid3D() {}

   // Displace p to a randomly chosen point on a sphere centered on the 
   // initial value of p
   void displace(Position& p, double dist, RNG& rng) const
   {
      double ph = rng.yield()*M_2PI;
      double th = acos(2.0*rng.yield()-1.0);
      double st = sin(th);

      p.x += dist*cos(ph)*st;
      p.y += dist*sin(ph)*st;
      p.z += dist*cos(th);
   }

   // Same as displace()
   void step(Position& p, double step, RNG& rng) const
   {
      displace(p, step, rng);
   }
}; 

}  // end DLA

#endif  // DLA_FLUID_3D_H
