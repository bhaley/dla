// ============================================================================
// Lattice2D.h -- DLA::Lattice2D class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_LATTICE_2D_H
#define DLA_LATTICE_2D_H

#include "Medium.h"  // <cmath>

namespace DLA {

// Medium for 2D diffusion; (x,y) are multiples of a_latt
class Lattice2D : public Medium
{
   public:

   // Constructor
   explicit Lattice2D(double a = 1.0) : Medium(a) {}

   // noncopyable prevents copy constructor and copy assignment

   // Destructor
   ~Lattice2D() {}

   // Displace p to a randomly chosen point on a circle centered on the 
   // initial value of p
   void displace(Position& p, double dist, RNG& rng) const
   {
      double ph = rng.yield()*M_2PI;

      p.x += next_whole(dist*cos(ph)*inv_a_latt)*a_latt;
      p.y += next_whole(dist*sin(ph)*inv_a_latt)*a_latt;
   }

   // Move p to a randomly chosen neighbor lattice site; step ignored
   void step(Position& p, double step, RNG& rng) const
   {
      switch (static_cast<int>(rng.yield()*4.0))
      {
         case 0:
            p.x += a_latt;
            break;
         case 1:
            p.x -= a_latt;
            break;
         case 2:
            p.y += a_latt;
            break;
         case 3:
            p.y -= a_latt;
            break;
      }
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

#endif  // DLA_LATTICE_2D_H
