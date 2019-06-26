// ============================================================================
// Timer.h -- DLA::Timer class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_TIMER_H
#define DLA_TIMER_H

#include <sys/time.h>

namespace DLA {

// Record elapsed time with microsecond resultion
class Timer
{
   public:

   // Constructor
   Timer()
   {}

   // Synthesized copy constructor, copy assignment, and destructor

   // Start timing
   void start(void)
   {
      t0 = seconds();
   }

   // Return elapsed time since start()
   double time(void)
   {
      return seconds() - t0;
   }

   private:

   // Return current time in seconds, with microseconds
   double seconds(void)
   {
      gettimeofday(&tv, NULL);
      return tv.tv_sec + 1e-6*tv.tv_usec;
   }

   struct timeval tv;
   double t0;
};

}  // end DLA

#endif  // DLA_TIMER_H
