// ============================================================================
// Cluster.cc -- DLA::Cluster methods
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#include <boost/format.hpp>
#include "Cluster.h"

using std::vector;

// ============================================================================
// Setup density members (density_dr, density_count, and density_corr)
// ============================================================================
void 
DLA::Cluster::density_setup(double max_dist, double dr)
{
   density_dr = dr;
   if (dr > 0.0)
   {
      int n = static_cast<int>(max_dist/dr);

      for (int i = 0; i < n; i++)
      {
         density_count.push_back(0);
         density_corr.push_back(0.0);
      }
   }
}
   
// ============================================================================
// Add a newly captured particle; update density_count if density_dr > 0.0
// ============================================================================
void 
DLA::Cluster::add_position(DLA::Position& p)
{
   DLA::PositionTree::iterator pit = ptree.begin();
   double r;

   if (0 == curr_particles)
   {
      timer.start();
      seed = p;
   }
   while (pit != ptree.end())
   {
      r = pit->distance(p);
      rgyr_sum += r*r;
      if (density_dr > 0.0)
         density_count[static_cast<int>(r/density_dr)]++;
      pit++;
   }
   ptree.insert(p);
   curr_particles++;
   if (max_particles == curr_particles)
      growth_time = timer.time();
   r = p.distance(seed);
   if (r > radius)
      radius = r;
}

// ============================================================================
// Calculate density_corr
// ============================================================================
void
DLA::Cluster::finalize_density(const DLA::MediumPtr& m)
{
   const double norm = 
      (curr_particles > 0) ? 1.0/static_cast<double>(curr_particles) : 0.0;
   double SA;

   for (int i = 1; i < density_count.size(); i++)
   {
      SA = m->surface_area(static_cast<double>(i)*density_dr);
      density_corr[i] = static_cast<double>(density_count[i])*norm/SA;
   }
}

// ============================================================================
// Return 1 if the branch with root_index as its root has at least size
// particles, else return 0
// ============================================================================
int
DLA::Cluster::branch_size(int root_index, int size) const
{
   // TODO
}

// ============================================================================
// Write particle positions in XYZ format
// ============================================================================
void 
DLA::Cluster::write_xyz(std::ostream& os) const
{
   DLA::PositionTree::const_iterator pit = ptree.begin();
   int type;
   int np = 1;
   int i, nb;

   os << curr_particles << "\nCluster\n";
   while (pit != ptree.end())
   {
      // type = (branches[pit->index].size() > 1) ? 1 : 0;
      type = 0;
      nb = branches[pit->index].size();
      if (nb > 1)
      {
         type = 1;
         for (i = 0; i < nb; i++)
            type &= branch_size(branches[pit->index][i], 5);
      }
      os << boost::format("%5d%15.2f%15.2f%15.2f%5d\n") 
                          % np % pit->x % pit->y % pit->z % type;
#ifndef XYZ_SAME_INDEX
      if (++np == 100000)
         np = 1;
#endif
      pit++;
   }
}
