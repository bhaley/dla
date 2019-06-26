// ============================================================================
// Lattice3D.h -- DLA::Lattice3D class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_LATTICE_3D_H
#define DLA_LATTICE_3D_H

#include "Medium.h"  // <cmath>

namespace DLA {

// Medium for 3D diffusion; (x,y,z) are multiples of a_latt
class Lattice3D : public Medium
{
   public:

   // Constructor
   explicit Lattice3D(double a = 1.0) : Medium(a) {}

   // noncopyable prevents copy constructor and copy assignment

   // Destructor
   ~Lattice3D() {}

   // Displace p to a randomly chosen point on a sphere centered on the 
   // initial value of p
   void displace(Position& p, double dist, RNG& rng) const
   {
      double ph = rng.yield()*M_2PI;
      double th = acos(2.0*rng.yield()-1.0);
      double st = sin(th);

      p.x += next_whole(dist*cos(ph)*st*inv_a_latt)*a_latt;
      p.y += next_whole(dist*sin(ph)*st*inv_a_latt)*a_latt;
      p.z += next_whole(dist*cos(th)*inv_a_latt)*a_latt;
   }

   // Move p to a randomly chosen neighbor lattice site; step ignored
   void step(Position& p, double step, RNG& rng) const
   {
      switch (static_cast<int>(rng.yield()*6.0))
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
         case 4:
            p.z += a_latt;
            break;
         case 5:
            p.z -= a_latt;
            break;
      }
   }
}; 

}  // end DLA

#endif  // DLA_LATTICE_3D_H
