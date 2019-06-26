//
// Verify performance of DLA::RNG class relative to boost::uniform_01

#include <boost/random.hpp>
#include <iostream>
#include "Timer.h"
#include "RNG.h"

using std::cout;

int main(void)
{
#define SEED 12345
   DLA::RNG rng(SEED);
   DLA::Timer t;
   const int n = 5000000;
   const int m = 1000;
   int i, j;
   double t_rng, t_boost;
   boost::mt11213b mt_eng(SEED);
   boost::uniform_01<boost::mt11213b> brng(mt_eng);

   t_rng = 0.0;
   for (i = 0; i < m; i++)
   {
      t.start();
      for (j = 0; j < n; j++)
         rng.yield();
      t_rng += t.time();
   }

   t_boost = 0.0;
   for (i = 0; i < m; i++)
   {
      t.start();
      for (j = 0; j < n; j++)
         brng();
      t_boost += t.time();
   }

   cout << "Mean time to generate " << n << " random values in [0, 1), ";
   cout << m << " samples:\n";
   cout << "RNG: " << t_rng/(double)m << "\n";
   cout << "boost::uniform_01: " << t_boost/(double)m << "\n";
   cout << "Difference: " << t_boost/t_rng << "\n";
   return 0;
}
