                                    NearTree

                                 Release 3.1.1
                   23 April 2011 (revised 27 September 2011)
 (c) Copyright 2001, 2008, 2009, 2010, 2011 Larry Andrews. All rights reserved
                                    based on
         Larry Andrews, "A template for the nearest neighbor problem",
   C/C++ Users Journal, Volume 19 , Issue 11 (November 2001), 40 - 49 (2001),
                                ISSN:1075-2838,
                        www.ddj.com/architect/184401449

   Revised 12 Dec 2008, for sourceforge release, Larry Andrews and Herbert J.
                                   Bernstein
                       8 Jan 2009 Release 1.0 LCA and HJB
                     11 Jan 2009 Release 1.0.1 LCA and HJB
                     21 March 2009 Release 2.0 LCA and HJB
                      30 May 2009 Release 2.1 LCA and HJB
                     4 June 2009 Release 2.1.1 LCA and HJB
                     7 June 2009 Release 2.1.2 LCA and HJB
                     7 July 2009 Release 2.1.3 LCA and HJB
                       29 November 2009 Release 2.1.4 LCA
                    23 April 2010 Release 2.1.5 LCA and HJB
                          18 July 2010 Release 2.2 HJB
                         25 July 2010 Release 2.2.1 HJB
                         31 August 2010 Release 2.3 LCA
                       7 September 2010 Release 2.3.1 LCA
                       30 October 2010 Release 2.3.2 LCA
                     22 March 2011 Release 3.0 LCA and HJB
                     5 April 2011 Release 3.0.1 LCA and HJB
                        19 April 2011 Release 3.0.2 HJB
                         23 April 2011 Release 3.1 HJB
                      27 September 2011 Release 3.1.1 HJB

    YOU MAY REDISTRIBUTE NearTree UNDER THE TERMS OF THE LGPL

+------------------------------------------------------------------------------+
|                                 LGPL NOTICES                                 |
|                                                                              |
| This library is free software; you can redistribute it and/or modify it      |
| under the terms of the GNU Lesser General Public License as published by the |
| Free Software Foundation; either version 2.1 of the License, or (at your     |
| option) any later version.                                                   |
|                                                                              |
| This library is distributed in the hope that it will be useful, but WITHOUT  |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU* Lesser General Public License |
| for more details.                                                            |
|                                                                              |
| You should have received a copy of the GNU Lesser General Public License     |
| along with this library; if not, write to the Free Software Foundation,      |
| Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA                 |
+------------------------------------------------------------------------------+

   This is a release of an API for finding nearest neighbors among points in
   spaces of arbitrary dimensions. This release provides a C++ template,
   TNear.h, and a C library, CNearTree.c, with example/test programs.

   Release 3.1.1 adjusted the libtool version from 5:0:1 to 6:0:1 to avoid
   confusion on the SONAME of the library as requested by Teemu Ikonen for
   use as a Debian package.

   Release 3.1 adjusted the randomization to be based on the depth rather
   than the population and added an optional detailed height calculation.

   Release 3.0.2 added to randomization on insertion when the tree is not
   well balanced.

   Release 3.0.1 updated the diameter calculation and fixed some
   documentation errors.

   Release 3.0 (formerly named Release 2.4) is a major change to NearTree,
   restructuring the default search from left-first to balanced and adding
   hooks to collect information about the tree.

   Release 2.3.2 adds optional returns of vectors of ordinals of found
   objects

   Release 2.3.1 adds Centroid method for Lloyd clustering.

   Release 2.3 added methods for clustering.

   Release 2.2.1 was a minor revision to Release 2.2 to add an include of
   limits.h to TNear.h, primarily for MINGW use.

   Release 2.2 added support for C code for fixed length string searches
   using a hamming distance norm, and for spherical and hemispherical
   geodesic norm based searches. Because of the addition of new type and norm
   flags, the version 2.2 shared libraries cannot be used to support binaries
   compiled against earlier headers and vice-versa.

   Release 2.1.5 was a cleanup update to the 2.1 release of 30 May 2009 to
   increase portability, in five stages (2.1.1 on 4 June 2009, 2.1.2 on 7
   June 2009, 2.1.3 on 7 July 2009, 2.1.4 on 29 November 2009 and 2.1.5 on 23
   April 2010) dealing with the following issues:
     * Convert to use of a self-contained portable random-number generator
       from Rob Harrison (2.1.1)
     * Ensure wider use of const where appropriate (2.1.1)
     * Correct typos and unclear wording in the README (2.1.2)
     * Reorganize use of USE_LOCAL_HEADERS in CNearTreeTest.cpp (2.1.2)
     * Change FAR macro to CNEARTREE_FAR (2.1.3)
     * Add BelongsToPoints and SeparateByRadius (2.1.4)
     * Fix dimensions for rhrand (2.1.5)

   The 2.1 release was a minor update to the 2.0 release of 21 March 2009 to
   deal with the following issues:
     * Make delayed insertion the default
     * Complete the containerization of TNear.h
     * Add code for K-nearest/farthest in TNear.h and in CNearTree.c
     * Correct the InAnnulus search filter

   Release 2.0 was a major update to the 1.0 release of 8 January 2009 to
   deal with the following issues:
     * Replace use recursion with a stack, except in insertion logic
     * Replace use of double with templated DistanceType (usually double)
     * Provide constuctors to build NearTree from vectors, lists or sets
     * Change "Insert" to "insert" for consistency with other containers
     * Add access function "at" or array type references [], and provide
       contents of a neartree as a vector
     * Add iterator support
     * Provide delayed insertion logic
     * Functions added for searches outside of a sphere or in an annular
       region

   Our thanks to Nicolas Brodu for suggesting the more general handling of
   the distance type.

   Note: As Nicolas Brodu has noted, CNearTree is particularly well-suited to
   multi-threaded applications. However, if the same CNearTree is to be
   searched in multiple threads, it is important to complete all insertions
   and/or delayed insertions before parallel execution of parallel searches.

     ----------------------------------------------------------------------

    Contents

     * Installation
     * The C++ template: TNear.h
     * The C NearTree API: CNearTree.c
     * A Portable pseudo-random number generator: rhrand.h

     ----------------------------------------------------------------------

     ----------------------------------------------------------------------

    Installation

   The NearTree package is available at
   www.sourceforge.net/projects/neartree. A source tarball is available at
   downloads.sourceforge.net/neartree/NearTree-3.1.1.tar.gz. Later tarballs
   may be available.

   If you decide to simply use the TNear.h header to add nearest neighbor
   support to C++ code under Visual Studio, be sure to also use the rhrand.h
   and triple.h headers. It is no longer necessary to define
   USE_LOCAL_HEADERS, which is automatically defined if _MSC_VER is defined.
   For unix or MINGW, you will need to use the Makefile and to have libtool
   on your system. Be warned that the default libtool under Mac OS X will not
   work for this installation.

   When the source tarball is downloaded and unpacked, you should have a
   directory NearTree-3.1.1. To see the current settings for a build execute

   make

   which should give the following information:

  PLEASE READ README_NearTree.txt and lgpl.txt
 
  Before making the NearTree libraries and example programs, check
  that the chosen settings are correct
 
  The current C++ and C compile commands are:
 
    libtool --mode=compile g++ -g -O2  -Wall -ansi -pedantic  \
       -DCNEARTREE_SAFE_TRIANG=1 -I.   -c
    libtool --mode=compile gcc -g -O2  -Wall -ansi -pedantic  \
       -DCNEARTREE_SAFE_TRIANG=1 -I.   -c
 
  The C API, CNearTree.c, depends on the sourceforge project CVector
  You are currently setup to use the system defaults for CVector
  If that is not correct, define the variable CVECTOR_INCLUDE

  The current library link command is:
 
    libtool --mode=link  gcc -version-info 6:0:1  \
      -no-undefined -rpath /usr/local/lib
 
  The current C++ and C library local, and C dynamic and static build commands are:
 
    libtool --mode=link g++ -no-undefined -g -O2  -Wall -ansi -pedantic \
       -DCNEARTREE_SAFE_TRIANG=1 -I.
    libtool --mode=link gcc -g -O2  -Wall -ansi -pedantic \
       -DCNEARTREE_SAFE_TRIANG=1 -I.
    libtool --mode=link gcc -no-undefined -g -O2  -Wall -ansi -pedantic \
       -DCNEARTREE_SAFE_TRIANG=1 -shared -I/usr/local/include
    libtool --mode=link gcc -g -O2  -Wall -ansi -pedantic  \
       -DCNEARTREE_SAFE_TRIANG=1 -static-libtool-libs -I/usr/local/include
 
  Before installing the NearTree library and example programs, check
  that the install directory and install commands are correct:
 
  The current values are :
 
    /usr/local
    libtool --mode=install cp
    
 
  To compile the NearTree library and example programs type:
 
    make clean
    make all
 
  To run a set of tests type:
 
    make tests
 
  To clean up the directories type:
 
    make clean
 
  To install the library and headers type:
 
    make install


   If these settings need to be changed, edit Makefile. On some systems, e.g.
   Mac OS X, the default libtool is not appropriate. In that case you should
   install a recent version of libtool. The CVector kit has been tested with
   libtool versions 1.3.5 and 1.5.4. For MINGW, libtool version 2.2.6 and gcc
   version 4 are needed to work with shared libraries (DLLs). If the system
   libtool is not to be used, define the variable LIBTOOL to be the path to
   the libtool executable, e.g. in bash

   export LIBTOOL=$HOME/bin/libtool

   or in the Makefile

   LIBTOOL = $(HOME)/bin/libtool

   If you need to include local header files using #include "..." instead of
   #include <...>, define the variable USE_LOCAL_HEADERS. USE_LOCAL_HEADERS
   is the default for Visual Studio under Microsoft Windows.

   Optionally, you may also define CNEARTREE_FORCEFLIP to maximize tree
   reorganization on insertion, CNEARTREE_NOFLIP to suppress tree
   reorganization on insertion, CNEARTREE_NODEFER to make all insertions
   immediate, CNEARTREE_FORCEPREPUNE to do searches first with a tighter
   estimate on the search radius, and CNEARTREE_NOPREPRUNE to suppress that
   behavior. The defaults are to do tree reorganization on insertion, to
   defer insertions, but not to preprune the search radius.

   If you define CNEARTREE_INSTRUMENTED, code will be enabled to track node
   visits in searching the tree.

   The triangle inequality that must be evaluated in building trees and
   retrieving data may not be evaluated correctly if the range of the three
   values is extremely large (>10**15 or so for doubles) or may be evaluated
   differently by some compilers in different parts of a program (due to
   differing usage of registers). The default in this API is to do the
   triangle inequality three different ways under the control of
   CNEARTREE_SAFE_TRIANG

 #ifdef CNEARTREE_SAFE_TRIANG
 #define TRIANG(a,b,c) (  (((b)+(c))-(a) >= 0) \
                       || ((b)-((a)-(c)) >= 0) \
                       || ((c)-((a)-(b)) >= 0))   
 #else
 #define TRIANG(a,b,c) (  (((b)+(c))-(a) >= 0))
 #endif

   Problems with the unsafe definition of TRIANG have been seen in Linux
   under gcc version 4 and in MS Window under VS 2003. There is a slight
   performance hit from the triple test. If maximal speed is critical and
   misidentification of nearest points by relative distance errors of about 1
   part in 10**15 is not a serious problem, the definition of
   -DCNEARTREE_SAFE_TRIANG=1 can be removed from the definition of CFLAGS in
   the Makefile.

   NOTE: A range of 10**15 is comparable to the diameter of the earth vs. the
   separation of two bonded atoms.

     ----------------------------------------------------------------------

     ----------------------------------------------------------------------

    The C++ template: TNear.h

   This is a revised release of

     template <typename T, typename DistanceType=double, int distMinValue=-1
     > class CNearTree;

   implementing the Nearest Neighbor algorithm after Kalantari and McDonald,
   (IEEE Transactions on Software Engineering, v. SE-9, pp. 631-634,1983)
   modified to use recursion for insertions and recursion (original version)
   or a stack (current version) for searches instead of a double-linked tree
   and simplified. The default search algorithm no longer favors the left
   branch first, but follows the more balanced Kalantari and McDonald
   approach. The prior search algorithm is available in "Left" versions of
   the search routines doing a bit less checking for things like is the
   distance to the right less than the distance to the left.

   This template is used to contain a collection of objects. After the
   collection has been loaded into this structure, it can be quickly queried
   for which object is "closest" to some probe object of the same type. The
   major restriction on applicability of the near-tree is that the algorithm
   only works if the objects obey the triangle inequality. The triangle rule
   states that the length of any side of a triangle cannot exceed the sum of
   the lengths of the other two sides.

   CNearTree is the root class for the neartree. The actual data of the tree
   is stored in NearTreeNode objects descending from a CNearTree.

   The types of objects that can be stored in the tree is quite broad. The
   biggest limitation is that the objects must reside in some sort of metric
   space and must obey the triangle rule. They must also be all of the same
   size because they are stored in an std::vector. If your application
   requires objects of varying storage, then your best way to use this code
   is to store pointers or handles and to write your own distance functions.
   Note that std::string is a pointer type variable and so can be stored
   directly.

   The type of the objects to be stored is the only required template
   argument. The type of the distance measure (DistanceType) defaults to
   double. If your application is for an integer type, then the type for
   DistanceType can be your integer type. This has the potential for speeding
   the calculations by avoiding FP computation. Other general types can be
   used if desired, but you may need to also input a value of distMinValue.

   The template argument distMinValue must be something that your class will
   understand as a negative number. The default input is negative one.
   Internally, that is cast to DistanceType. Since most uses will be for
   DistanceType to be double, that is a simple conversion. Obviously, for
   integer types, there is no problem either. The need for this value is to
   have something internally that is recognizable as smaller than the
   smallest "distance" that can exist between any two objects in your type.
   For most users, there is no need to input anything other than the default,
   -1. -1 must be castable to DistanceType. It seems unlikely that anyone
   would actually need this optional parameter, but it is here for
   completeness.

   It is a design decision that this class cannot work for unsigned types.
   Verifying the triangle rule for unsigned types is more complex. Sorry,
   unsigned types are left as an exercise for the reader.

   The user of this class needs to provide at least the following
   functionality for the template to work. For the built-in numerics of C++,
   they are provided by the system.

 DistanceType Norm( void ); 
                            // a function "Norm( void )" of the templated class 
                            // to return DistanceType (usually will return a    
                            // "length" of type double)                         
 operator- ( );             // geometrical (vector) difference of two objects   
                            // a copy constructor would be nice                 
                            // a constructor would be nice                      
                            // a destructor would be nice                       

   The provided interface is:

     #include <TNear.h>

     CNearTree( void )   // constructor
        instantiated by something like:      CNearTree <T> vTree;
        for some type T
       
     CNearTree( const ContainerType<T> & o)    // constructor from containers, e.g. ...

     CNearTree( const std::vector<T> & o )     // constructor
     CNearTree( const std::list<T> & o )       // constructor
     CNearTree( const std::set<T> & o )        // constructor
     CNearTree( const CNearTree<T> & o )       // constructor

     CNearTree( const ContainerType<T> & o1,
                       const ContainerType<T> & o2)
                                                               // constructor merging 2 containers, e.g. ...
    
     ~CNearTree( void )  // destructor
    
     void clear( void )  // clear the NearTree
    
     long GetFlags( void ) const                       // Get all execution flags
     void SetFlags( const long flags )                 // Set all execution flags
     long GetFlags( const long mask ) const     // Get execution flags within mask
     void SetFlags( const long flags, const long mask )
                                                                            // Set execution flags within mask
                                                                           
     // The available execution flags are
    
     static const long        NTF_NoPrePrune        = 1; //flag to suppress all search prepruning
     static const long        NTF_ForcePrePrune     = 2; //flag to force search prepruning
     static const long        NTF_NoFlip            = 4; //flag to suppress flips on insert
     static const long        NTF_ForceFlip         = 8; //flag to force flips on insert
     static const long        NTF_NoDefer           =16; //flag to prevent deferred insert

    
     template<typename InputContainer>
     CNearTree& operator=( const InputContainer& o )
                                                               // put container's contents into a NearTree,
                                                               // wiping out the current contents
    
     template<typename InputContainer>
     CNearTree& operator=( InputContainer& o )
                                                               // put container's contents into a NearTree,
                                                               // wiping out the current contents

     template<typename InputContainer>
     CNearTree& operator+=( const InputContainer& o )
                                                               // add a container's contents to a NearTree

     template<typename InputContainer>
     CNearTree& operator-=( const InputContainer& o )
                                                               // remove a container's contents from a NearTree

     template<typename InputContainer>
     CNearTree& set_symmetric_difference( const InputContainer&, o )
                                                               // remove the part of a container's
                                                               // contents from a NearTree that is
                                                               // already in the Neartree and add
                                                               // in the contents of the container
                                                               // that is not already in the Neartree
                                                               // i.e. the exclusive or
                                                              
    

     void insert( const T& t )
        where t is an object of the type T

             all inserts are delayed until a search is performed or
             until an explicit call to CompleteDelayedInsertions
             is called or a search is called. The purpose is to distribute
             the objects a bit more  randomly. Excessively ordered objects
             leads to less than optimal trees.
      
             Starting with the 2.1 release, places objects in a queue for
             insertion later when  CompleteDelayInsert is called.  In
             earlier releases the default was immediate insertion.
            
             The following additional convenience insert template
             allow insertion of containers of objects


     template< typename InputContainer >
     void insert( ContainerType & o )      // e. g. ...

     void insert( const std::vector<T> & o )
     void insert( const std::list<T> & o )
     void insert( const std::set<T> & o )
     void insert( const CNearTree<T> & o )

     iterator NearestNeighbor ( const DistanceType & dRadius, const T& t ) const
        returns an iterator to the nearest point to the probe point t or end() if there is none
     bool NearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t ) const
        dRadius is the largest radius within which to search; make it
           very large if you want to include every point that was loaded.
        tClosest is returned as the object that was found closest to the probe
           point (if any were within radius dRadius of the probe)
        t is the probe point, used to search in the group of points insert'ed

        return value is true if some object was found within the search radius, false otherwise.
            If false is returned, tClosest is invalid (at best).


     iterator FarthestNeighbor ( T& const T& t ) const
        returns an iterator to the nearest point to the probe point t or end() if there is none
     bool FarthestNeighbor ( T& tFarthest,   const T& t ) const
        tFarthest is returned as the object that was found farthest from the probe
           point
        t is the probe point, used to search in the group of points Insert'ed
        return value is true if some object was found, false otherwise
           If false is returned, tFarthest is invalid (at best).

     iterator LeftNearestNeighbor ( const DistanceType & dRadius, const T& t ) const
        returns an iterator to the nearest point to the probe point t or end() if there is none
     bool LeftNearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t ) const
        dRadius is the largest radius within which to search; make it
           very large if you want to include every point that was loaded.
        tClosest is returned as the object that was found closest to the probe
           point (if any were within radius dRadius of the probe)
        t is the probe point, used to search in the group of points insert'ed

        return value is true if some object was found within the search radius, false otherwise.
            If false is returned, tClosest is invalid (at best).


     iterator LeftFarthestNeighbor ( T& const T& t ) const
        returns an iterator to the nearest point to the probe point t or end() if there is none
     bool LeftFarthestNeighbor ( T& tFarthest,   const T& t ) const
        tFarthest is returned as the object that was found farthest from the probe
           point
        t is the probe point, used to search in the group of points Insert'ed
        return value is true if some object was found, false otherwise
           If false is returned, tFarthest is invalid (at best).

     The "Left..." versions of NearestNeighbor and FarthestNeighbor are deprecated versions
     provided for compatibility with earlier releases of NearTree.  There are also "Short..."
     and "LeftShort..." versions of NearestNeighbor to support experimental prepruning logic.


     The following functions (BelongsToPoints, SeparateByRadius, FindInSphere, FindOutSphere,
     and FindInAnnulus) all return a container (ContainerType) that can be any standard library
     container (such as std::vector< T >) or CNearTree.  In each case option arguments
     allow a parallel vector of indices to be returned for each container, giving the
     indices of the returned objects within the original NearTree.
     
     template<typename ContainerType>
     void BelongsToPoints ( const T& t1, const T& t2,
         ContainerType& group1, ContainerType& group2 )
     template<typename ContainerType>
     void BelongsToPoints ( const T& t1, const T& t2,
         ContainerType& group1, ContainerType& group2,
         std::vector<size_t>& group1_ordinals, std::vector<size_t>& group2_ordinals)
        returns the points closer to t1 than to t2 in group1 and the rest in group 2
        if group1_ordinals and group2_ordinals are provided the ordinals of the
        found objects in the object store are put into those vectors. The ordinals
        can be used as indices into the CNearTree itself.
       
     template<typename ContainerTypeInside, typename ContainerTypeOutside>
     void SeparateByRadius ( const DistanceType radius, const T& probe,
         ContainerTypeInside& inside, ContainerTypeOutside& outside )
     template<typename ContainerTypeInside, typename ContainerTypeOutside>
     void SeparateByRadius ( const DistanceType radius, const T& probe,
         ContainerTypeInside& inside, ContainerTypeOutside& outside,
         std::vector<size_t>& inside_ordinals, std::vector<size_t>& outside_ordinals)
        return the points within radius of the probe in inside and the rest in outside
        if inside_ordinals and outside_ordinals are provided the ordinals of the
        found objects in the object store are put into those vectors.  The ordinals
        can be used as indices into the CNearTree itself.


     long FindInSphere ( const DistanceType& dRadius, 
          ContainerType& tInside,   const T& t ) const
     long FindInSphere ( const DistanceType& dRadius, 
          ContainerType& tInside,  
          std::vector<size_t>& tIndices, const T& t ) const
     long FindInSphere ( const DistanceType& dRadius,
          CNearTree<  T >& tInside,  const T& t ) const
     long LeftFindInSphere ( const DistanceType& dRadius, 
          ContainerType& tInside,   const T& t ) const
     long LeftFindInSphere ( const DistanceType& dRadius, 
          ContainerType& tInside,  
          std::vector<size_t>& tIndices, const T& t ) const
     long LeftFindInSphere ( const DistanceType& dRadius,
          CNearTree<  T >& tInside,  const T& t ) const
        dRadius is the radius within which to search; make it very large if you want to
            include every point that was loaded;
        tInside is returned as the NearTree or container of objects that were found within a radius dRadius
           of the probe point
        if the tIndices argument is given it will be returned as a vector
           of indices in the near tree of the objects returned.
        t is the probe point, used to search in the group of points Insert'ed

        return value is the count of the number of points found within the search radius
        the "Left..." versions are deprecated versions provided for compatibility with
        earlier NearTree releases.

     long FindOutSphere ( const DistanceType& dRadius,
          ContainerType& tOutside,   const T& t ) const
     long FindOutSphere ( const DistanceType& dRadius,
          ContainerType& tOutside,
          std::vector<size_t>& tIndices, const T& t ) const
     long FindOutSphere ( const DistanceType& dRadius,
          CNearTree<  T >& tOutside,   const T& t ) const
     long LeftFindOutSphere ( const DistanceType& dRadius,
          ContainerType& tOutside,   const T& t ) const
     long LeftFindOutSphere ( const DistanceType& dRadius,
          ContainerType& tOutside,
          std::vector<size_t>& tIndices, const T& t ) const
     long LeftFindOutSphere ( const DistanceType& dRadius,
          CNearTree<  T >& tOutside,   const T& t ) const
        dRadius is the radius outside of which to search
        tOutside is returned as the NearTree or container of objects that were found at
           or outside of radius dRadius of the probe point
        if the tIndices argument is given it will be returned as a vector
           of indices in the near tree of the objects returned.
        t is the probe point, used to search in the group of points Insert'ed

        return value is the count of the number of points found outside the search radius
        the "Left..." versions are deprecated versions provided for compatibility with
        earlier NearTree releases.
    
     long FindInAnnulus ( const DistanceType& dRadius1,
          const DistanceType& dRadius2,
          ContainerType& tInRing,   const T& t ) const
     long FindInAnnulus ( const DistanceType& dRadius1,
          const DistanceType& dRadius2,
          ContainerType& tInRing,
          std::vector<size_t>& tIndices,  const T& t ) const
     long FindInAnnulus ( const DistanceType& dRadius1,
          const DistanceType& dRadius2,
          CNearTree<  T >& tInRing,   const T& t ) const
     long LeftFindInAnnulus ( const DistanceType& dRadius1,
          const DistanceType& dRadius2,
          ContainerType& tInRing,   const T& t ) const
     long LeftFindInAnnulus ( const DistanceType& dRadius1,
          const DistanceType& dRadius2,
          ContainerType& tInRing,
          std::vector<size_t>& tIndices,  const T& t ) const
     long LeftFindInAnnulus ( const DistanceType& dRadius1,
          const DistanceType& dRadius2,
          CNearTree<  T >& tInRing,   const T& t ) const
        dRadius1 and  dRadius2 are the two radii between which to find data points
        tInRing is returned as the NearTree or container of objects that were found at
           or outside of a radius dRadius1 and at or inside of radius dRadius2 of the probe point
        if the tIndices argument is given it will be returned as a vector
           of indices in the near tree of the objects returned.
        t is the probe point, used to search in the group of points Insert'ed

        return value is the count of the number of points found within the annulus
        the "Left..." versions are deprecated versions provided for compatibility with
        earlier NearTree releases.
    
     long FindK_NearestNeighbors ( const size_t k, const DistanceType& dRadius,
          ContainerType& tClosest, const T& t )
     long FindK_NearestNeighbors ( const size_t k, const DistanceType& dRadius,
          ContainerType& tClosest,
          std::vector<size_t>& tIndices, const T& t )
     long FindK_NearestNeighbors ( const size_t k, const DistanceType& dRadius,
          CNearTree<  T >& tClosest, const T& t )
     long LeftFindK_NearestNeighbors ( const size_t k, const DistanceType& dRadius,
          ContainerType& tClosest, const T& t )
     long LeftFindK_NearestNeighbors ( const size_t k, const DistanceType& dRadius,
          ContainerType& tClosest,
          std::vector<size_t>& tIndices, const T& t )
     long LeftFindK_NearestNeighbors ( const size_t k, const DistanceType& dRadius,
          CNearTree<  T >& tClosest, const T& t )
        k is the maximum number of nearest neighbors to return. Finds this many if possible
        dRadius within a sphere defined by dRadius, to search for the k-nearest-neighbors
        tClosest is returned as the ContainerType or NearTree of the objects found
        if the tIndices argument is given it will be returned as a vector
           of indices in the near tree of the objects returned.
        t is the probe point, used to search in the group of points insert'ed

        return value is the count of the number of points found within the sphere
        the "Left..." versions are deprecated versions provided for compatibility with
        earlier NearTree releases.


     long FindK_FarthestNeighbors ( const size_t k,
          ContainerType& tFarthest, const T& t )
     long FindK_FarthestNeighbors ( const size_t k,
          ContainerType& tFarthest,
          std::vector<size_t>& tIndices, const T& t )
     long FindK_FarthestNeighbors ( const size_t k,
           CNearTree<  T >& tFarthest, const T& t )
     long LeftFindK_FarthestNeighbors ( const size_t k,
          ContainerType& tFarthest, const T& t )
     long LeftFindK_FarthestNeighbors ( const size_t k,
          ContainerType& tFarthest,
          std::vector<size_t>& tIndices, const T& t )
     long LeftFindK_FarthestNeighbors ( const size_t k,
           CNearTree<  T >& tFarthest, const T& t )
        k is the maximum number of farthest neighbors to return. Finds this many if possible
        tFarthest is returned as the ContainerType or NearTree of the objects found
        if the tIndices argument is given it will be returned as a vector
           of indices in the near tree of the objects returned.
        t is the probe point, used to search in the group of points insert'ed

        return value is the count of the number of points found within the sphere
        the "Left..." versions are deprecated versions provided for compatibility with
        earlier NearTree releases.

   

     ----------------------------------------------------------------------


 Access Functions:

      T at ( const size_t n ) const
         returns the n'th item of the internal data store.  This is not
         guaranteed to be in the order of insertion.

      T operator[] ( const size_t n )
         returns the n'th item of the internal data store.  This is not
         guaranteed to be in the order of insertion.

      template<typename ContainerType>
      operator ContainerType ( void ) const
         returns all of the inserted objects in the tree in a container of type ContainerType.
         ContainerType can be std::vector<T>, etc, or other containers, including CNearTree<T>.
         The returned vector contents are not guaranteed to be returned in the order loaded.

      iterator begin ( void ) const
         returns an iterator to the beginning of the internal data store

      iterator end ( void ) const
         returns an iterator to the end of the data store (one beyond the last item)

      iterator back ( void ) const
         returns an iterator to the last data item of the internal data store


     ----------------------------------------------------------------------


 Information and special operation functions:

      void ImmediateInsert( const T&aamp; t )
         insert places objects in a queue for insertion later when CompleteDelayInsert
         is called or a search is called. ImmediateInsert inserts the data immediately
         into the tree (with the potential of a less balanced tree). ImmediateInsert is not
         intended for the ordinary user.

      void CompleteDelayedInsert ( void )
         completes insertion for all delayed objects. sqrt(n) of them are inserted by
         random choice.  The rest are inserted in linear order as originally queued.
         CompleteDelayedInsert is invoked at the beginning of all searches, so the
         average user will never need to call it.

      size_t GetDeferredSize ( void )
         returns the number of delayed objects that have not yet completed insertion. This is
         mainly for information about details of the tree.

      size_t GetTotalSize ( void )
         returns the number of objects that have been insert'ed plus those DelayInsert'ed

      size_t size ( void )
         identical to GetTotalSize

      size_t GetDepth ( void )
         returns the maximum tree layers from the root.  This is mainly for information about
         details of the tree.
        
      double GetDimEstimate ( void )  // returns an estimate of the Hausdorff dimension
      double GetDimEstimate ( const double DimEstimateEsd )
                                                     // returns an estimate of the Hausdorff dimension
                                                     // to within the given esd
      double GetDimEstimateEsd ( void )
                                                     // returns an estimate of the esd

      double GetDiamEstimate ( void )  // returns an estimate of the diameter

      DistanceType GetMeanSpacing ( void )   // returns an estimate object spacing
      DistanceType GetVarSpacing ( void )   // returns an estimate object spacing variance
     
      size_t GetNodeVisits ( void )   // returns the number of node visits if
                                                    // CNEARTREE_INSTRUMENTED as defined, 0 otherwise
      void SetNodeVisits,/b> ( const size_t visits)
                                                    // set the number of node visits
     

        
      T Centroid ( void )
         returns the centroid of a neartree.

      bool empty ( void )
         returns true if the tree is empty, otherwise false
        

     ----------------------------------------------------------------------


 Iterators:

      Random access iterators are provided for accessing the data in a CNearTree. The most important
      expected use is to retrieve the objects returned from one of the sphere search functions that
      can return a CNearTree. However, they can be used with any CNearTree.
     
      They should function in a fashion essentially the same as STL iterators. There is no assurance
      that data will be returned in the order it was loaded, just that it is accessible.  This is the
      list of iterators. The same set is available for const_iterator.

      iterator ( void ) { }; // constructor
      iterator ( const const_iterator& s )
      iterator& operator=   ( const iterator& s )     
      iterator& operator=   ( const const_iterator& s )     
      iterator  operator++  ( const int n )           
      iterator  operator--  ( const int n )           
      iterator& operator++  ( void )                  
      iterator& operator--  ( void )                  
      iterator  operator+   ( const long n ) const    
      iterator  operator-   ( const long n ) const    
      iterator& operator+=  ( const long n )          
      iterator& operator-=  ( const long n )          
      T         operator*   ( void )        const    

      bool      operator==  ( const iterator& t ) const
      bool      operator!=  ( const iterator& t ) const
      bool      operator==  ( const const_iterator& t ) const
      bool      operator!=  ( const const_iterator& t ) const
      bool      operator>  ( const iterator& t ) const
      bool      operator>  ( const const_iterator& t ) const
      bool      operator<  ( const iterator& t ) const
      bool      operator<  ( const const_iterator& t ) const


      const T * const operator->  ( void )   const


     ----------------------------------------------------------------------

  

   So a complete program is:

  #include "TNear.h"
  #include <cstdio>
  void main()
  {
    CNearTree< double > dT;
    double dNear;
    dT.Insert( 1.5 );
    if ( dT.NearestNeighbor( 10000.0,   dNear,  2.0 )) printf( "%f\n",double(dNear-2.0) );
  }

   and it should print 0.5 (that's how for 2.0 is from 1.5). For more
   examples of the use of TNear.h, see main.cpp and CNearTreeTest.cpp.

     ----------------------------------------------------------------------

     ----------------------------------------------------------------------

    The C NearTree API: CNearTree.c

    Synopsis

     #include <CNearTree.h>

     double CNearTreeDistsq ( CNearTreeHandle treehandle, void * coord1, void
     * coord2 );

     double CNearTreeDist ( CNearTreeHandle treehandle, void * coord1, void *
     coord2 );

     int CNearTreeSetNorm ( const CNearTreeHandle treehandle, int treenorm );

     int CNearTreeNodeCreate ( const CNearTreeHandle treehandle,
     CNearTreeNodeHandle * treenodehandle )

     int CNearTreeCreate ( CNearTreeHandle * treehandle, size_t treedim, int
     treetype );

     int CNearTreeFree ( const CNearTreeHandle treehandle );

     int CNearTreeClear ( CNearTreeHandle * treehandle );

     int CNearTreeNodeFree ( CNearTreeNodeHandle * treenodehandle );

     int CNearTreeInsert( const CNearTreeHandle treehandle, const void *
     coord, const void * obj );

     int CNearTreeImmediateInsert ( const CNearTreeHandle treehandle, const
     void * coord, const void * obj );

     int CNearTreeDelayedInsert ( const CNearTreeHandle treehandle, const
     void * coord, const void * obj ); /* ***DEPRECATED*** */

     int CNearTreeNodeInsert ( const CNearTreeHandle treehandle,
     CNearTreeNodeHandle treenodehandle, size_t index; size_t * depth );

     int CNearTreeNodeInsert_Flip ( const CNearTreeHandle treehandle,
     CNearTreeNodeHandle treenodehandle, size_t index; size_t * depth );

     int CNearTreeNodeReInsert_Flip ( const CNearTreeHandle treehandle, const
     CNearTreeNodeHandle treenodehandle, const CNearTreeNodeHandle pntn,
     size_t * depth );

     int CNearTreeCompleteDelayedInsert ( const CNearTreeHandle treehandle )
     int CNearTreeZeroIfEmpty ( const CNearTreeHandle treehandle );

     int CNearTreeGetSize ( const CNearTreeHandle treehandle, size_t * size
     );

     int CNearTreeGetTotalSize ( const CNearTreeHandle treehandle, size_t *
     size ); /* ***DEPRECATED*** */

     size_t CNearTreeSize ( const CNearTreeHandle treehandle);

     int CNearTreeGetDeferredSize ( const CNearTreeHandle treehandle, size_t
     * size );

     int CNearTreeGetDelayedSize ( const CNearTreeHandle treehandle, size_t *
     size ); /* ***DEPRECATED*** */

     int CNearTreeGetDepth ( const CNearTreeHandle treehandle, size_t * depth
     )

     int CNearTreeGetFlags ( const CNearTreeHandle treehandle, long * flags,
     const long mask )

     int CNearTreeSetFlags ( const CNearTreeHandle treehandle, const long
     flags, const long mask )

     int CNearTreeGetMeanSpacing ( const CNearTreeHandle treehandle, double *
     spacing );

     int CNearTreeGetVarSpacing ( const CNearTreeHandle treehandle, double *
     varspacing );

     int CNearTreeCount ( const CNearTreeHandle treehandle, size_t * count );

     int CNearTreeNodeCount ( const CNearTreeNodeHandle treenodehandle,
     size_t * count );

     #ifdef CNEARTREE_INSTRUMENTED

     int CNearTreeGetNodeVisits ( const CNearTreeHandle treehandle, size_t *
     visits);

     int CNearTreeSetNodeVisits ( const CNearTreeHandle treehandle, const
     size_t visits );

     #endif

     int CNearTreeGetDiamEstimate ( const CNearTreeHandle treehandle, double
     * diamest );

     int CNearTreeGetDimEstimateEsd ( const CNearTreeHandle treehandle,
     double * dimestesd );

     int CNearTreeGetDimEstimate ( const CNearTreeHandle treehandle, double *
     dimest, const double DimEstimateEsd );

     int CNearTreeNearestNeighbor ( const CNearTreeHandle treehandle, const
     double dRadius, void * * coordClosest, void * * objClosest, const void *
     coord );

     int CNearLeftTreeNearestNeighbor ( const CNearTreeHandle treehandle,
     const double dRadius, void * * coordClosest, void * * objClosest, const
     void * coord ); /* ***DEPRECATED*** */

     int CNearTreeFarthestNeighbor ( const CNearTreeHandle treehandle, void *
     * coordFarthest, void * * objFarthest, const void * coord );

     int CNearTreeFindInSphere ( const CNearTreeHandle treehandle, const
     double dRadius, CVectorHandle coordInside, CVectorHandle objInside,
     const void * coord, int resetcount );

     int CNearTreeFindTreeInSphere ( const CNearTreeHandle treehandle, const
     double dRadius, CNearTreeHandle foundInside, const void * coord, int
     resetcount )

     int CNearTreeFindOutSphere ( const CNearTreeHandle treehandle, const
     double dRadius, CVectorHandle coordOutside, CVectorHandle objOutside,
     const void * coord, int resetcount );

     int CNearTreeFindTreeOutSphere ( const CNearTreeHandle treehandle, const
     double dRadius, CNearTreeHandle foundOutside, const void * coord, int
     resetcount )

     int CNearTreeFindInAnnulus ( const CNearTreeHandle treehandle, const
     double dRadiusInner, const double dRadiusOuter, CVectorHandle
     coordInRing, CVectorHandle objInRing, const void * coord, int resetcount
     );

     int CNearTreeFindTreeInAnnulus ( const CNearTreeHandle treehandle, const
     double dRadiusInner, const double dRadiusOuter, CNearTreeHandle
     foundInRing, const void * coord, int resetcount )

     int CNearTreeFindKNearest ( const CNearTreeHandle treehandle, const
     size_t k, const double dRadius, CVectorHandle coordClosest,
     CVectorHandle objClosest, const void * coord, int resetcount );

     int CNearTreeFindKTreeNearest ( const CNearTreeHandle treehandle, const
     size_t k, const double dRadius, CNearTreeHandle foundClosest, const void
     * coord, int resetcount )

     int CNearTreeFindKFarthest ( const CNearTreeHandle treehandle, const
     size_t k, const double dRadius, CVectorHandle coordFarthest,
     CVectorHandle objFarthest, const void * coord, int resetcount );

     int CNearTreeFindKTreeFarthest ( const CNearTreeHandle treehandle, const
     size_t k, const double dRadius, CNearTreeHandle foundFarthest, const
     void * coord, int resetcount )

     int CNearTreeNearest ( const CNearTreeHandle treehandle, double *
     dRadius, void * * coordClosest, void * * objClosest, const void * coord
     );

     int CNearTreeLeftNearest ( const CNearTreeHandle treehandle, double *
     dRadius, void * * coordClosest, void * * objClosest, const void * coord
     ); /* ***DEPRECATED*** */

     int CNearTreeFindFarthest ( const CNearTreeHandle treehandle, double *
     dRadius, void * * coordFarthest, void * * objFarthest, const void *
     coord );

     int CNearTreeObjects ( const CNearTreeHandle treehandle, CVectorHandle *
     vectorhandle );

     void * CNearTreeObjectAt ( const CNearTreeHandle treehandle, size_t
     index );

     int CNearTreeCoords ( const CNearTreeHandle treehandle, CVectorHandle *
     vectorhandle );

     void * CNearTreeCoordAt ( const CNearTreeHandle treehandle, size_t index
     );

   The NearTree API works with coordinate vectors in an arbitrary number of
   dimensions. Each neartree is accessed by a pointer of type CNearTreeHandle
   which points to a struct of type CNearTree, which points to a tree of
   nodes of type CNearTreeNode:

     typedef struct _CNearTreeNode {
         size_t           m_indexLeft;    /* index of left coords in m_CoordStore 
                                             and of left object in m_ObjectStore     */
         size_t           m_indexRight;   /* index of right coords in m_CoordStore
                                             and of right object in m_ObjectStore     */
         double           m_dMaxLeft;     /* longest distance from the left object
                                             to anything below it in the tree            */
         double           m_dMaxRight;    /* longest distance from the right object
                                             to anything below it in the tree            */
         struct _CNearTreeNode * m_pLeftBranch; 
                                          /* tree descending from the left object        */
         struct _CNearTreeNode * m_pRightBranch;
                                          /* tree descending from the right object       */
         int              m_iflags;       /* flags                                       */
     } CNearTreeNode;
    
    
     typedef CNearTreeNode * CNearTreeNodeHandle;  
    
     typedef struct {
         CNearTreeNodeHandle m_ptTree;     /* pointer to the actual tree                  */
         size_t           m_szdimension;   /* dimension of the coordinates                */
         size_t           m_szsize;        /* size of this tree                           */
         size_t           m_szdepth;       /* depth of this tree                          */
         int              m_iflags;        /* flags                                       */
         CVectorHandle    m_ObjectStore;   /* all inserted objects                        */
         CVectorHandle    m_CoordStore;    /* all inserted coordinates                    */
         CVectorHandle    m_DelayedIndices;/* objects queued for insertion                */
     } CNearTree;
    
     typedef CNearTree     FAR * CNearTreeHandle;
    
     /*  Execution Control Flags */
    
 #define CNTF_NOPREPRUNE    0x10000L     /*flag to suppress all search prepruning */
 #define CNTF_FORCEPREPRUNE 0x20000L     /*flag to force search prepruning       */
 #define CNTF_NOFLIP        0x40000L     /*flag to suppress flips on insert      */
 #define CNTF_FORCEFLIP     0x80000L     /*flag to force flips on insert         */
 #define CNTF_NODEFER      0x100000L     /*flag to prevent deferred insert       */



   The internal operation of the API depends on the function CNearTreeDist
   that returns the distance (L1, L2 or L-infinity) between two coordinate
   vectors as a double according to the parameters of the given tree. Note
   that the tree may store the coordinates as integers or as doubles, but the
   distance is always computed as a double. If this function is replaced by a
   user function, it is important that the replacement obey the triangle
   inequality.

   A neartree is created by CNearTreeCreate and freed by CNearTreeFree.
   treedim is the dimension of the coordinate vectors and treetype is one of
   the three predefined constants CNEARTREE_TYPE_DOUBLE for double or
   CNEARTREE_TYPE_INTEGER for integer or CNEARTREE_TYPE_STRING, optionally
   ORed with CNEARTREE_NORM_L1, CNEARTREE_NORM_L2 or CNEARTREE_NORM_LINF for
   L1, L2 or L-infinity norms, CNEARTREE_NORM_SPHERE or
   CNEARTREE_NORM_HSPHERE for a spherical or hemispherical norm (L1-norm
   combination of radial and spherical/hemispherical triangle distances), or
   CNEARTREE_NORM_HAMMING for the string-Hamming distance norm (add one for
   each differing character position).

   Starting with release 2.1, all insertions are delayed by default, unless
   the insertions is done by a call to CNearTreeImmediateInsert. The
   insertions that have been queued are completed by a call to
   CNearTreeCompleteDelayedInsert or by any search. The insertions are
   actually done in a randomized order, either for an initial block of
   sqrt(#queue) by default. or for the entire queue if the flag
   CNEARTREE_DEFER_ALL is ored with treetype.

   Starting with release 3 (formerly called release 2.4) optionally, you may
   also define CNEARTREE_FORCEFLIP to maximize tree reorganization on
   insertion, CNEARTREE_NOFLIP to suppress tree reorganization on insertion,
   CNEARTREE_NODEFER to make all insertions immediate, CNEARTREE_FORCEPREPUNE
   to do searches first with a tighter estimate on the search radius, and
   CNEARTREE_NOPREPRUNE to suppress that behavior. The defaults are to do
   tree reorganization on insertion, to defer insertions, but not to preprune
   the search radius. If you define CNEARTREE_INSTRUMENTED, code will be
   enabled to track node visits in searching the tree.

   The flags CNEARTREE_DEFER_ALL and CNEARTREE_FLIP used in prior releases
   are deprecated, but are still defined. They have no effect.

   When first created, a neartree has no right or left node and with the
   dMax-below set to negative values so that any match found will be stored
   since it will greater than the negative value. The tree is then populated
   by calls to CNearTreeInsert, with each call providing a coordinate vector
   coord and an optional object pointer obj. The API copies the coordinate
   vector, but does not copy the object. Later, when a search is requested or
   an explicit call to CNearTreeCompleteDelayedInsert is made, the tree is
   populated in the order left, right and then the nearer child, working from
   a randomized selection from the items queued for insertion.

   Optionally, the actual insertions may done immediately by calling
   CNearTreeImmediateInsert instead of CNearTreeInsert. For upwards
   compatibility of the library for existing code, the deprecated
   CNearTreeDelayedInsert is provided as an deprecated alternate call to
   CNearTreeInsert.

   The neartree is searched for the nearest or farthest coordinate vector in
   the neartree to a given probe coordinate vector coord by
   CNearTreeNearestNeighbor and CNearTreeFarthestNeighbor, respectively.
   Starting with release 3, the search is balanced, following the left or
   right branch first depending on which child node is closest. The former
   left-first behavior is deprecated, but still available in
   CNearLeftTreeNearestNeighbor. The given radius confines the search to a
   sphere around the probe. If more than a single extremal coordinate point
   is needed, CNearTreeFindInSphere can be used to obtain a CVector result
   vector of all the coordinate vectors that satisfy the constraint of being
   within a specified radius, or CNearTreeFindOutSphere can be used to obtain
   a CVector result vector of all the coordinates that satisfy the constraint
   of being outside a specified radius. CNearTreeFindIn Annulus can be used
   to obtain a CVector result vector of all the coordinates that satisfy the
   constraint of being between two specified radii from the probe.
   CNearTreeFindKNearest can be used to obtain a CVector result vector of the
   k coordinates closest to the probe point such that all results are within
   the specified radius of the probe point, or CNearTreeFindKFarthest to
   obtain a CVector result vector of the k coordinates farthest from the
   probe point such that all results are at or outside the specified radius
   of the probe point. The vectors themselves are not copied into the result
   vector. If the parameter resetcount is true (non zero) the result vector
   is cleared before the search. A CVector result vector of the matching
   object pointers is returned if objs is not NULL. Aternatively the forms
   CNearTreeFindTreeInSphere, CNearTreeFindTreeOutSphere,
   CNearTreeFindTreeInAnnulus, CNearTreeFindKTreeNearest,
   CNearTreeFindKTreeFarthest can be used to obtain CNearTrees rather than
   CVectors of results. The functions CNearTreeNearest and
   CNearTreeFindFarthest implement CNearTreeNearestNeighbor and
   CNearTreeFarthestNeighbor, respectively, adjusting the radius of the
   search while the search is in progress and are not normally used by users.

   The size of the tree as a count of objects can be obtained using the
   function NearTreeGetSize or the macro NearTreeSize. The size of the tree
   as a count of nodes and the depth of the tree can be obtained using the
   functions CNearTreeCount and CNearTreeGetDepth. Estimates of the Hausdorff
   dimension, the esd of that estimate, the diameter, the spacing and the
   variance of the spacing can be obtained with CNearTreeGetDimEstimate,
   CNearTreeGetDimEstimateEsd, CNearTreeGetDiamEstimate,
   CNearTreeGetMeanSpacing and CNearTreeGetVarSpacing.

    Returns

   If CNearTreeDist fails, it returns -1. Except for CNearTreeDist, all the
   functions in the API return 0 ( CNEARTREE_SUCCESS ) for success. If
   dynamic memory allocation fails, CNEARTREE_MALLOC_FAILED is returned. If a
   call is made with an improper argument, CNEARTREE_BAD_ARGUMENT is
   returned. If a search fails to find anything, CNEARTREE_NOT_FOUND is
   returned. If there is a failure in an attempt to free a CNearTree,
   CNEARTREE_FREE_FAILED is returned. If any of the internal call to CVector
   fail, CNEARTREE_CVECTOR_FAILED is returned. For convenience in debugging,
   the formerly negative values of these returns are now positive.

    Examples

   To create a neartree for 3-dimensional vectors of doubles:

 
 #include <CNearTree.h>
 CNearTreeHandle treehandle;
 int bReturn;
 
  ...
 
  bReturn = !CNearTreeCreate(&treehandle,3,CNEARTREE_TYPE_DOUBLE);

   To insert a copy of a 3-dimensional vector of doubles into this tree, with
   no associated object:

 
     double v[3];
 
  ...
     
     v[0] = 1.; v[1] = 2.; v[2] = 3.;
     bReturn = !CNearTreeInsert(treehandle,&v[0],NULL);

   To search for the nearest neighbor to a probe vector vSearch in a radius
   of 3., returning a pointer to the resulting vector in vBest:

 
     double * vBest;
     void * vvBest;
     double vSearch[3];
     double   dRad = =3.;
    
   ...
  
     if ( !CNearTreeNearestNeighbor(treehandle,dRad,&vvBest,NULL,vSearch))
         {   vBest = (double *)vvBest; }

   Note the use of a separate void * vvBest instead of a cast of &vBest to
   avoid compiler type punning warnings.

   For more examples of the use of CNearTree.c, see main.c and
   CNearTreeTest.c in the release kit.

     ----------------------------------------------------------------------

     ----------------------------------------------------------------------

    A Portable pseudo-random number generator: rhrand.h

   rhrand.h is a portable pseudo-random number generator based one by Rob
   Harrison, derived from "one in J.M.Hammersley and D.C. Handscomb, 'Monte
   Carlo Methods,' Methuen & Co., London and Wiley & Sons, New York, 1964,
   p47". See also, D. E. Knuth "The Art of Computer Programming", Volume 2,
   "Seminumerical Alogorithms, Third Edition, Addison-Wesley, Reading MA,
   1997.

   rhrand.h is a header file in which a C++ class, RHrand, is defined, and a
   C struct typedef CRHrand is defined.

   The C++ interface is

     static const int RHRAND_MAX = 32767;  /* the integer range accessible as RHrand::RHRAND_MAX */
    
     RHrand(void)                          /* the default constructor */
    
     RHrand( const int iseed )             /* a constructor to start with the given seed */
    
     ~RHrand( void)                        /* a destructor */
    
     void srandom( const int iseed)        /* reset the generator based on the given seed */
    
     double urand( void )                  /* return a random double uniformly distributed in [0,1) */
    
     int random ( void )                   /* return a random integer uniformly distributed in [0, RHRAND_MAX-1] */


   In C++ code, typical use is

 #include <rhhand.h>
     RHrand rhr;

 ...

     x = rhr.urand();

   The C interface is suppressed in RHRAND_NOCCODE is defined. Otherwise the
   C interface is based on defining a struct of type CRHRrand and calling
   macros that refer to a handle of type RCRHrandHandle.

     typedef struct CRHrand_ {           /* the struct used in random number generattion */
         double buffer[55];
         int indx;
         int jndx;
         int kndx;
         double dTemp;
     } CRHrand;

     typedef CRHrand * CRHrandHandle;     /* the type to be used in maro calls */

     #define CRHRAND_MAX 32767            /* the integer range */
    
     #define CRHrandSrandom(randhandle,iseed) ... 
                                          /* a macro to call to initialize CHRrandHandle randhandle
                                             using see int iseed */
                                            
     #define CRHrandUrand(randhandle) ... /* a macro to return a random double uniformly distributed in [0,1) */
    
     #define CRHrandRandom(randhandle) ((int)(CRHrandUrand(randhandle)*(double)CRHRAND_MAX))
                                          /* a macro to return a random integer uniformly distributed in
                                             [0, CRHRAND_MAX-1] */

   Typical use is

 #include <rhhand.h>
     CRHrand rhr;

 ...

     CRHrandSrandom(&rhr, 0 );

 ...

     x = CRHrandUrand(&rhr);

     ----------------------------------------------------------------------

     ----------------------------------------------------------------------

   Updated 27 September 2011
   andrewsl@ix.netcom.com
