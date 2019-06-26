// ============================================================================
// Fluid2D.h -- DLA::Fluid2D class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_FLUID_2D_H
#define DLA_FLUID_2D_H

#include "Medium.h"  // <cmath>

namespace DLA {

// Medium for 2D diffusion; (x,y) can take any value
class Fluid2D : public Medium
{
   public:

   // Constructor
   Fluid2D() : Medium(1.0) {}

   // noncopyable prevents copy constructor and copy assignment

   // Destructor
   ~Fluid2D() {}

   // Displace p to a randomly chosen point on a cirlce centered on the 
   // initial value of p
   void displace(Position& p, double dist, RNG& rng) const
   {
      double ph = rng.yield()*M_2PI;

      p.x += dist*cos(ph);
      p.y += dist*sin(ph);
   }

   // Same as displace()
   void step(Position& p, double step, RNG& rng) const
   {
      displace(p, step, rng);
   }

   // Return the area of a circle of radius r
   double volume(double r) const
   {
      return M_PI*r*r;
   }

   // Return the circumference of a circle of radius r
   double surface_area(double r) const
   {
      return M_2PI*r;
   }

}; 

}  // end DLA

#endif  // DLA_FLUID_2D_H
