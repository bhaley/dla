// ============================================================================
// RNG.h -- DLA::RNG class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_RNG_H
#define DLA_RNG_H

#include <boost/random/mersenne_twister.hpp>
#include <boost/utility.hpp>
#include <cmath>
#include <ctime>
#include "Position.h"

namespace DLA {

// Wrapper for Boost Mersenne Twister PRNG (period 2^11213-1) to give
// uniformly distributed random doubles in [0, 1)
class RNG : private boost::noncopyable
{
   public:

   // Constructor: 0 seed implies using the current time
   explicit RNG(uint32_t seed = 0) : rng()
   {
      rng.seed((seed > 0) ? seed : static_cast<uint32_t>(time(NULL)));
   }

   // noncopyable prevents copy constructor and copy assignment

   // Synthesized destructor

   // Return a uniformly distributed pseudo-random double in [0, 1)
   double yield(void) 
   {
      return static_cast<double>(rng())*2.3283064365386963e-10;
   }

   private:

   boost::mt11213b rng;
}; 

}  // end DLA

#endif  // DLA_RNG_H
