// ============================================================================
// Medium.h -- DLA::Medium class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_MEDIUM_H
#define DLA_MEDIUM_H

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include "Position.h"
#include "RNG.h"

#define M_4PI_3 4.18879020478369
#define M_2PI   6.28318530717959
#define M_4PI   12.5663706143592

namespace DLA {

// Base class for specific media
class Medium : private boost::noncopyable
{
   public:

   // Constructor
   explicit Medium(double a = 1.0) : a_latt(a), inv_a_latt(1.0/a) {}

   // noncopyable prevents copy constructor and copy assignment

   // Destructor
   virtual ~Medium() {}

   // Displace p to a randomly chosen point from the initial value of p;
   // this represents the large (dist >> a_latt) initial displacement of
   // a walker from Cluster seed particle to start diffusion
   virtual void displace(Position& p, double dist, RNG& rng) const = 0;

   // Move p to a randomly chosen point one step from the initial value of p;
   // this represents a single diffusive step
   virtual void step(Position& p, double step, RNG& rng) const = 0;

   // Return the volume of a sphere of radius r; 2D media redefine for area
   virtual double volume(double r) const
   {
      return M_4PI_3*r*r*r;
   }

   // Return the surface area of a sphere of radius r; 2D media redefine for
   // circumference
   virtual double surface_area(double r) const
   {
      return M_4PI*r*r;
   }

   protected:

   // Return the next whole number as far or farther from 0 as r
   double next_whole(double r) const
   {
      return (r > 0.0) ? ceil(r) : floor(r);
   }

   double a_latt;  // lattice parameter
   double inv_a_latt;
}; 

typedef boost::shared_ptr<Medium> MediumPtr;

}  // end DLA

#endif  // DLA_MEDIUM_H
