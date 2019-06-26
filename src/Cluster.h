// ============================================================================
// Cluster.h -- DLA::Cluster class
// ----------------------------------------------------------------------------
// Author: Benjamin P. Haley
// Copyright (c) 2019 Benjamin P. Haley
// ----------------------------------------------------------------------------
// See the LICENSE file for information on usage and redistribution of this
// file and for a DISCLAIMER OF ALL WARRANTIES.
// ============================================================================

#ifndef DLA_CLUSTER_H
#define DLA_CLUSTER_H

#include <boost/utility.hpp>
#include <vector>
#include <ostream>
#include <cmath>
#include "TNear.h"
#include "Position.h"
#include "Timer.h"
#include "Params.h" // MediumPtr

namespace DLA {

typedef CNearTree<Position> PositionTree;

// Aggregate of particles 
class Cluster : private boost::noncopyable
{
   public:

   // Constructor: ptree, timer, radius_growth, density_count, density_corr,
   // and rgyr_growth are initialized by their default ctors
   explicit Cluster(int nparticles): 
      max_particles(nparticles), curr_particles(0), num_walkers(0), 
      captured_steps(0), radius(0.0), growth_time(0.0), density_dr(0.0),
      rgyr_sum(0.0), branches(nparticles) {}

   // noncopyable prevents copy constructor and copy assignment

   // Destructor: member dtors delete everything
   ~Cluster() {}

   // Return the number of particles in this Cluster
   int get_size(void) const
   {
      return curr_particles;
   }

   // Return the current radius
   double get_radius(void) const
   {
      return radius;
   }

   // Return the current radius of gyration
   double get_radius_gyration(void) const
   {
      double N = static_cast<double>(curr_particles);

      return (N > 0.0) ? sqrt(0.5*rgyr_sum/(N*N)) : 0.0;
   }

   // Return the current radius vs. particles vector
   const std::vector<double>& get_radius_growth(void) const
   {
      return radius_growth;
   }

   // Return the current radius vs. particles vector
   const std::vector<double>& get_radius_gyration_growth(void) const
   {
      return rgyr_growth;
   }

   // Return the current density vs. r correlation vector
   const std::vector<double>& get_density_corr(void) const
   {
      return density_corr;
   }

   // Return the current fraction of all walkers that are captured
   double get_capture_fraction(void) const
   {
      return (num_walkers > 0) ? 
         static_cast<double>(curr_particles)/static_cast<double>(num_walkers) :
         0.0;
   }

   // Return the mean free steps of walkers before capture
   double get_mean_free_steps(void) const
   {
      return (curr_particles > 0) ?
        static_cast<double>(captured_steps)/static_cast<double>(curr_particles):
         0.0;
   }

   // Return the time in seconds between the addition of the first and last
   // particles
   double get_growth_time(void) const
   {
      return growth_time;
   }

   // Setup density members (density_dr, density_count, and density_corr)
   void density_setup(double max_dist, double dr);
   
   // Record a new walker (not necessarily captured)
   void add_walker(void)
   {
      num_walkers++;
   }

   // Record the free steps of a captured walker
   void add_steps(int nsteps)
   {
      captured_steps += nsteps;
   }

   // Add a newly captured particle; update density_count if density_dr > 0.0
   void add_position(Position& p);

   // Add a branch: indexth particle is attached to the parent_indexth particle
   void add_branch(int parent_index, int index)
   {
      branches[parent_index].push_back(index);
   }

   // Record growth properties for current size
   void update_growth(void)
   {
      radius_growth.push_back(radius);
      rgyr_growth.push_back(get_radius_gyration());
   }

   // Calculate density_corr
   void finalize_density(const MediumPtr& m);

   // Return true if any particle is near p (within search_radius) and set 
   // pnear to the nearest particle position
   bool find_nearest(const Position& p, Position& pnear, double search_radius)
   {
      return ptree.NearestNeighbor(search_radius, pnear, p);
   }

   // Write particle positions in XYZ format
   void write_xyz(std::ostream& os) const;

   private:

   // Return 1 if the branch with root_index as its root has at least size
   // particles, else return 0
   int branch_size(int root_index, int size) const;

   PositionTree ptree;
   Position seed;
   int max_particles;
   int curr_particles;
   int num_walkers;
   int captured_steps;
   double radius;
   double growth_time;
   double density_dr;
   double rgyr_sum;
   std::vector<double> radius_growth;
   std::vector<double> rgyr_growth;
   std::vector<int> density_count;
   std::vector<double> density_corr;
   std::vector<std::vector<int> > branches;
   Timer timer;
};

}  // end DLA

#endif  // DLA_CLUSTER_H
