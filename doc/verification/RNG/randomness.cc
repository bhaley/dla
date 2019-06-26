//
// Produce binary sequence data, rng.data, as input for NIST Statistical 
// Test Suite (STS).

#include <fstream>
#include "RNG.h"

// 15,000 values * 1,000 generators = 15,000,000 32-bit/4-byte integers
// 60,000,000 bytes = 60 1,000,000 byte sequences

#define N   1000    // Number of generators
#define M  15000    // Number of values (sequences x length)
#define SEED 12345  // Base seed */

int main(void)
{
   DLA::RNG *rngs[N];
   uint32_t seed = SEED;
   uint32_t val;
   int i, j;
   std::ofstream of("rng.data", std::ios::out | std::ios::binary);
   // The NIST STS tests binary sequences in which, at any point, 0 or 1 is 
   // equally likely.  Use integers in [0, 2^32) rather than floating point
   // values for testing.
   const double rng_rescale = 4294967296.0;

   for (i = 0; i < N; i++, seed++)
      rngs[i] = new DLA::RNG(seed);

   for (j = 0; j < M; j++)
   {
      for (i = 0; i < N; i++)
      {
         val = (uint32_t)(rngs[i]->yield()*rng_rescale);
         of.write((char *)&val, sizeof(uint32_t));
      }
   }
   of.close();

   for (i = 0; i < N; i++)
      delete rngs[i];
   return 0;
}
