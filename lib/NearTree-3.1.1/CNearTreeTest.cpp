//*
//*  CNearTreeTest.cpp
//*  NearTree
//*
//*  test harness for the templated neartree implementation, TNear.h
//*  Copyright 2008 Larry Andrews.  All rights reserved
//*
//*  Revised 29 November 2009, added SeparateByRadius and BelongsToPoint
//*                            to be used for cluster analysis. 
//*                            BelongsToPoint simply runs thru all the
//*                            because that is the fastest way
//*  Revised 30 May 2009, release with full containerization of C++
//*                       version and KNear/Far in C++ and C, LCA + HJB
//*  Revised 13 Nov 2010, revisions to C++ version for balanced
//*                       searches, LCA+HJB



//**********************************************************************
//*                                                                    *
//* YOU MAY REDISTRIBUTE NearTree UNDER THE TERMS OF THE LGPL          *
//*                                                                    *
//**********************************************************************/

//************************* LGPL NOTICES *******************************
//*                                                                    *
//* This library is free software; you can redistribute it and/or      *
//* modify it under the terms of the GNU Lesser General Public         *
//* License as published by the Free Software Foundation; either       *
//* version 2.1 of the License, or (at your option) any later version. *
//*                                                                    *
//* This library is distributed in the hope that it will be useful,    *
//* but WITHOUT ANY WARRANTY; without even the implied warranty of     *
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *
//* Lesser General Public License for more details.                    *
//*                                                                    *
//* You should have received a copy of the GNU Lesser General Public   *
//* License along with this library; if not, write to the Free         *
//* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,    *
//* MA  02110-1301  USA                                                *
//*                                                                    *
//**********************************************************************/

/*
 This is a test harness for the templated neartree implementation, CNearTree.
 
 */

#include <algorithm>
#include <utility>

#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef USE_LOCAL_HEADERS
#include <TNear.h>
#include <rhrand.h>
#else
#include "TNear.h"
#include "rhrand.h"
#endif
#include <list>
#include <set>
#include <vector> 
#include <ctime>

#define ABS(x) (((x)<0)?-(x):(x))

RHrand rhr;

void testSeparation( void );
void testEmptyTree( void );
void testLinearTree( const int n );
void testFindFirstObject( void );
void testFindLastObject( void );
void testFindInSphereFromBottom( void );
void testBackwardForward( void );
void testFindInSphereFromTop( void );
void testOutSphere( void );
void testDelayedInsertion( void );
void testDelayedInsertionRandom( void );
void testIterators( void );
void testFindInAnnulus( void );
void testMisc( void );

void testRandomTree1( const int n );
void testRandomTree2( const int n );
void testBigVector( void );
void testIntegerReturn( void );
void testBigIntVec( void );
void testSTLContainerInput( void );
void testKNearFar( void );
void testMergeConstructor( void );
void testOperatorPlusEquals( void );
void testOperatorMinusEquals( void );
void testSetSymmetricDifference( void );
void testCentroid( void );
void testLloyd( void );

//std::vector<int> LloydCycleStep( const CNearTree<int>& coord, const std::vector<int>& vIn );

long g_errorCount;

int debug;

#define timecommand(idstring,command) \
{ \
clock_t tc1 = std::clock();\
{command} \
clock_t tc2 = std::clock();\
fprintf( stdout, "%s time %f \n", idstring, ((double)(tc2-tc1))/CLOCKS_PER_SEC); \
}

#ifndef CNEARTREE_INSTRUMENTED
#define timetreecommand(tree,idstring,command) \
timecommand(idstring,command)
#else
#define timetreecommand(tree,idstring,command) \
{ \
size_t nv1 = tree.GetNodeVisits(); \
clock_t tc1 = std::clock();\
{command} \
clock_t tc2 = std::clock();\
size_t nv2 = tree.GetNodeVisits(); \
fprintf( stdout, "%s time %f, visits %ld \n", idstring, ((double)(tc2-tc1))/CLOCKS_PER_SEC, (long)(nv2-nv1)); \
}

#endif

/*=======================================================================*/
int main(int argc, char* argv[])
{
    g_errorCount = 0;
    
    debug = 0;
    if (argc > 1 && !strcmp(argv[1],"--debug")) debug = 1;
   
        
    /* test the interface with an empty tree */
    testEmptyTree( );
    fprintf( stdout, "testEmptyTree\n" );
    
    /* test the interface with trees with varying content, one entry and several */
    for( int i=1; i<10; ++i )
    {
        char tagstring[40];
        sprintf(tagstring,"testLinearTree %d",i);
        testLinearTree( i );
    }

    timecommand("testSeparation",{testSeparation( );});
    timecommand("testFindFirstObject",{testFindFirstObject( );});
    timecommand("testFindLastObject",{testFindLastObject( );});
    testFindInSphereFromBottom( );
    fprintf( stdout, "testFindInSphereFromBottom\n" );
    testFindInSphereFromTop( );
    fprintf( stdout, "testFindInSphereFromTop\n" );
    testOutSphere( );
    fprintf( stdout, "testOutSphere\n" );
    testFindInAnnulus( );
    fprintf( stdout, "testFindInAnnulus\n" );
    testRandomTree1( 10000 );
    fprintf( stdout, "testRandomTree1\n" );
    testRandomTree2( 1000 );
    fprintf( stdout, "testRandomTree2\n" );
    testBigVector( );
    fprintf( stdout, "testBigVector\n" );
    testBigIntVec( );
    fprintf( stdout, "testBigIntVec\n" );
    testBackwardForward( );
    fprintf( stdout, "testBackwardForward\n" );
    testDelayedInsertion( );
    fprintf( stdout, "testDelayedInsertion\n" );
#ifndef CNEARTREE_NODEFER
    testDelayedInsertionRandom( );
    fprintf( stdout, "CompleteDelayedInsertRandom\n" );
#endif
    testIterators( );
    fprintf( stdout, "testIterators\n" );
    testIntegerReturn( );
    fprintf( stdout, "testIntegerReturn\n" );
    testMisc( );
    fprintf( stdout, "testMisc\n" );
    testSTLContainerInput( );
    fprintf( stdout, "testSTLContainerInput\n" );
    testKNearFar( );
    fprintf( stdout, "testKNearFar\n" );
    timecommand("testMergeConstructor",{testMergeConstructor( );});
    timecommand("testOperatorPlusEquals",{testOperatorPlusEquals( );});
    timecommand("testOperatorMinusEquals",{testOperatorMinusEquals( );});
    timecommand("testSetSymmetricDifference",{testSetSymmetricDifference( );});
    timecommand("testCentroid",{testCentroid( );});
    timecommand("testLloyd",{testLloyd( );});
    

    if( g_errorCount == 0 )
    {
        fprintf(stdout,  "No errors were detected while testing CNearTree\n" );
    }
    else 
    {
        fprintf(stdout,  "%ld Errors were detected while testing CNearTree\n", g_errorCount );
    }
    
    return g_errorCount;
}  // end main

/*
 For an empty tree of int's, test the public interface for CNearTree.
 */
/*=======================================================================*/
void testEmptyTree( void )
{
    CNearTree<int> tree;
    int close;
    int nFar;
    std::vector<int> v;
    bool bTreeEmpty;
    bool bTreeHasNearest;
    bool bTreeHasFarthest;
    long lFoundPointsInSphere;
    
    long myflags = tree.GetFlags();
    
    if ((myflags&CNearTree<int>::NTF_NoPrePrune)) fprintf(stdout, "Flag NTF_NoPrePrune set \n");
    if ((myflags&CNearTree<int>::NTF_ForcePrePrune)) fprintf(stdout, "Flag NTF_ForcePrePrune set \n");
    if ((myflags&CNearTree<int>::NTF_NoFlip)) fprintf(stdout, "Flag NTF_NoFlip set \n");
    if ((myflags&CNearTree<int>::NTF_ForceFlip)) fprintf(stdout, "Flag NTF_ForceFlip set \n");
    if ((myflags&CNearTree<int>::NTF_NoDefer)) fprintf(stdout, "Flag NTF_NoDefer set \n");
    
    bTreeEmpty = tree.empty( );
    if( ! bTreeEmpty )
    {
        ++g_errorCount;
        fprintf(stdout, "testEmptyTree: ..empty incorrect for empty tree\n" );
    }
    if( tree.size( ) != 0 )
    {
        ++g_errorCount;
        fprintf(stdout, "testEmptyTree: .size incorrect for empty tree\n" );
    }
    
    bTreeHasNearest  = tree.NearestNeighbor( 0.0, close, 1 );
    if( bTreeHasNearest )
    {
        ++g_errorCount;
        fprintf(stdout, "testEmptyTree: .NearestNeighbor incorrect for empty tree\n" );
    }
    
    bTreeHasFarthest = tree.FarthestNeighbor( nFar, 0 );
    if( bTreeHasFarthest )
    {
        ++g_errorCount;
        fprintf(stdout, "testEmptyTree: .FarthestNeighbor incorrect for empty tree\n" );
    }
    
    
    CNearTree<int> ntReturn;
    lFoundPointsInSphere = tree.FindInSphere( 1000.0, ntReturn, 1 );
    if( lFoundPointsInSphere != 0 )
    {
        ++g_errorCount;
        fprintf(stdout, "testEmptyTree: .FindInSphere incorrect for empty tree\n" );
    }
    
    tree.clear( );
}  // end testEmptyTree

/*
 Test CNearTree using a tree built only using int's. This is not a robust test
 in some ways because the tree is only composed of right branches and left leaves.
 Perform some general tests.
 */
/*=======================================================================*/
void testLinearTree( const int n )
{
    CNearTree<int> tree;
    
    /* generate an unbalanced tree*/
    for( int i1=1; i1<=n; ++i1 )
    {
        tree.insert( i1 );
    }
    
    if( tree.GetDepth( ) > (size_t)(n+1)/2 )
    {
        ++g_errorCount;
        fprintf(stdout, "testLinearTree: tree depth is too large, %lu is greater than %d\n", (unsigned long)tree.GetDepth( ), (n+1)/2 );
    }
    
#ifdef CNEARTREE_INSTRUMENTED
    if( tree.GetHeight( ) !=  tree.GetDepth( ) )
    {
        ++g_errorCount;
        fprintf(stdout, "testLinearTree: tree depth != tree height, %lu != %lu\n", (unsigned long)tree.GetDepth( ), (unsigned long)tree.GetHeight( ) );
    }
#endif
    
    size_t estdim = (size_t)(0.5+tree.GetDimEstimate());
    if (n < 129 && estdim != 0) {
       ++g_errorCount;   
       fprintf(stdout, "testLinearTree: dimension estimate %ld\n",(long)estdim);
    }
    
    /*
     Search for the nearest value using a probe point that is larger than 
     the largest value that was input. The returned values should be the
     last value entered into the tree. 
     */
    int closest=-1;   
    const bool bClose = tree.NearestNeighbor( 22.0, closest, 2*n );
    if( ! bClose )
    {
        ++g_errorCount;
        fprintf(stdout, "testLinearTree: NearestNeighbor failed to find anything\n" );
    }
    else if( closest != n )
    {
        ++g_errorCount;
        fprintf(stdout, "NearestNeighbor failed in testLinearTree, got %d, should be %d\n", closest, n );
    }
    
    /*
     Search for the farthest value using a probe point that is larger than 
     the largest value that was input. The returned values should be the
     first value entered into the tree. 
     */
    int farthest;
    const bool bFar = tree.FarthestNeighbor( farthest, 2*n );
    if( ! bFar )
    {
        ++g_errorCount;
        fprintf(stdout, "testLinearTree: FarthestNeighbor failed to find anything\n" );
    }
    else if( farthest != 1 )
    {
        ++g_errorCount;
        fprintf(stdout, "FarthestNeighbor failed in testLinearTree, got %d, should be %d\n", farthest, n );
    }
    
    /*
     Find all of the points in the tree using a negative radius, using the first
     input point as the probe point. Nothing should be found.
     */
    CNearTree<int> ntReturn;
    const long lFoundPointsInSphere = tree.FindInSphere( -100.0, ntReturn, 1 );
    if( lFoundPointsInSphere != 0 )
    {
        ++g_errorCount;
        fprintf(stdout, "FindInSphere found points for negative radius\n" );
    }
    
    /*
     Find all of the points in the tree using a small radius. Do separate
     searches for every point entered. In every case, only a single point
     should be found.
     */
    long localErrorCount = 0;
    long localErrorMax = 0;
    for( int i=1; i<=n; ++i )
    {
        ntReturn.clear( );
        const long found = tree.FindInSphere( 0.1, ntReturn, 1 );
        if( found != 1 )
        {
            ++localErrorCount;
            if( found > localErrorMax ) 
            {
                localErrorMax = found;
            }
        }
    }
    
    if( localErrorCount != 0 )
    {
        ++g_errorCount;
        fprintf(stdout, "FindInSphere found too many points (as many as %ld) %ld times\n", localErrorMax, localErrorCount );
    }
    
    /*
     Find all of the points in the tree that are within a large radius, using 
     the first input point as the probe point. All of the input points should 
     be found within the radius.
     */
    if( tree.FindInSphere( (double)(10*n), ntReturn, 0 ) != n )
    {
        ++g_errorCount;
        fprintf(stdout, "FindInSphere did not find all the points, found %ld\n", tree.FindInSphere( (double)(10*n), ntReturn, 0 ) );
    }
    
    /* test operator[ ] */
    for( size_t i=1; i<=(size_t)n; ++i )
    {
        if( i != (size_t)tree[i-1] )
        {
            ++g_errorCount;
            fprintf(stdout, "testLinearTree: found tree[i]!=i for tree[i-1]=%d, i=%ld\n", tree[i-1], (long)i );
        }
    }
    
    tree.clear( );
}  // end testLinearTree

/*
 Perform general tests using floating point numbers. Two test sets are
 included, one for float and one for double.
 The values are computed starting from some initial value, and
 each succeeding value is one half of the previous until "zero" is
 computed (the zero is NOT inserted into the tree). The tree will consist
 of only right branches and left leaves.
 
 Experience showed that differing implementations of behaviors for denormalized
 floating point numbers causes problems both with the tests and with the
 building of the tree. So FLT_MIN and DBL_MIN have replaced zero as the
 lower bounds.
 */
/*=======================================================================*/
void testFindFirstObject( void )
{
    {
        double fFinal = FLT_MAX; /* just initialization */
        CNearTree<float> tree;
        long count = 0;
        
        /* build the float tree starting with 1.0 */
        const float fInitial = 1.0;
        float f = fInitial;
        while( f > fInitial*FLT_EPSILON )
        {
            tree.insert( f );
            fFinal = f;
            f /= 2.0;
            ++count;
        }
        
        if( tree.empty( ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject incorrectly found empty tree for float\n" );
        }
        
       
        //   /*
        //   Search for the value closest to zero. It should be a very small, probably
        //   denormalized number.
        //   */
        float closest = 0.0;
        const bool bReturnNear = tree.NearestNeighbor( 1.5*fFinal, closest, 0.0 );
        if( ! bReturnNear )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject: Near failed to find anything\n" );
        }
        else if( ABS(closest - fFinal) > FLT_EPSILON )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject: Near failed for float, got %f\n", closest );
        }
        
        //   /*
        //   Search for the value farthest from a large number. It should be a
        //   very small, probably denormalized number.
        //   */
        float farthest = -1000.0;
        const bool bReturnFar = tree.FarthestNeighbor( farthest, 100.0 );
        if( ! bReturnFar )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject Far failed to any anything for floatn" );
        }
        else if( ABS(farthest - fFinal) > 100.*FLT_EPSILON )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject Far failed for float, got %g\n", farthest );
        }
        
        //   /*
        //   Determine if FindInSphere can find all of the input data.
        //   */
        CNearTree<float> sphereReturn;
        const long lFound = tree.FindInSphere( 100.0, sphereReturn, 1.0 );
        if( lFound != count )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject: found wrong count for FindInSphere for float, should be%ld, got %ld\n", count, lFound );
        }
    }
    
    {
        double dFinal = DBL_MAX; /* just initialization */
        CNearTree<double> tree;
        long count = 0;
        
        /* build the double tree starting with 1.0 */
        const double dInitial = 1.0;
        double f = dInitial;
        /* generate an unbalanced tree*/
        while( f > 0.0 && f >= dInitial*DBL_EPSILON )
        {
            tree.insert( f );
            dFinal = f;
            f /= 2.0;
            ++count;
        }
        
        if( tree.empty( ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject incorrectly found empty tree for double\n" );
        }
        
        /*
         Search for the value closest to zero. It should be a very small, probably
         denormalized number.
         */
        double closest = 0.0;
        const bool bReturnNear = tree.NearestNeighbor( 1.0e-10, closest, 0.0 );
        if( ! bReturnNear )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject: Near failed to find anything\n" );
        }
        else if( closest-dFinal != 0.0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject: Near failed for double, got %f\n", closest );
        }
        
        /*
         Search for the value farthest from a large number. It should be a
         very small, probably denormalized number.
         */
        double farthest = 10000.0;
        const bool bReturn = tree.FarthestNeighbor( farthest, 100.0 );
        if( ! bReturn )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject: Far failed to find anything for double\n"  );
        }
        else if( ABS(farthest-dFinal) > 100.*DBL_EPSILON )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject Far failed for double, got %g\n", farthest );
        }
        
        /*
         Determine if FindInSphere can find all of the input data.
         */
        CNearTree<double> sphereReturn;
        const long lFound = tree.FindInSphere( 100.0, sphereReturn, 1.0 );
        if( lFound != count )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindFirstObject: found wrong count for FindInSphere for double, should be%ld, got %ld\n", count, lFound );
        }
    }
}  // end testFindFirstObject

/*
 Perform general tests using floating point numbers.
 Build a tree of floats. The values are computed starting from some initial 
 value, and each succeeding value is one half of the previous until "zero"
 is computed (the zero is NOT inserted into the tree). The tree will consist
 of only right branches and left leaves.
 
 Experience showed that differing implementations of behaviors for denormalized
 floating point numbers causes problems both with the tests and with the
 building of the tree. So FLT_MIN and DBL_MIN have replaced zero as the
 lower bounds.
 */
/*=======================================================================*/
void testFindLastObject( void )
{
    {
        CNearTree<float> tree;
        
        float f = 1.0;
        /* generate an unbalanced tree*/
        while( f > 0.0  && f >= FLT_MIN )
        {
            tree.insert( f );
            f /= 2.0;
        }
        
        float closest = 0.0;
        const bool bReturn = tree.NearestNeighbor( 100.0, closest, 11.0 );
        if( ! bReturn )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject: Near failed to find anything\n" );
        }
        else if( closest != 1.0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject: Near failed for float, got %g\n", closest );
        }
    }
    
    {
        CNearTree<double> tree;
        
        double f = 1.0;
        /* generate an unbalanced tree*/
        while( f > 0.0 && f >= DBL_MIN)
        {
            tree.insert( f );
            f /= 2.0;
        }
        
        double closest = 0.0;
        const bool bReturn = tree.NearestNeighbor( 100.0, closest, 11.0 );
        if( ! bReturn )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject: Near failed to find anything\n" );
        }
        else if( closest != 1.0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject: Near failed for float, got %g\n", closest );
        }
    }
    
    {
        CNearTree<float> tree;
        
        float f = 1.0;
        /* generate an unbalanced tree*/
        while( f > 0.0 && f >= FLT_MIN)
        {
            tree.insert( f );
            f /= 2.0;
        }
        
        float closest = 0.0;
        const float search = -11.0;
        const bool bReturn = tree.FarthestNeighbor( closest, search );
        if( ! bReturn )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject: Far failed to find anything\n" );
        }
        else if( closest != 1.0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject: Far failed for float, got %f\n", closest );
        }
    }
    
    {
        CNearTree<double> tree;
        
        double f = 1.0;
        /* generate an unbalanced tree*/
        while( f > 0.0 && f >= DBL_MIN)
        {
            tree.insert( f );
            f /= 2.0;
        }
        
        double closest = 0.0;
        const bool bReturn = tree.FarthestNeighbor( closest, -11.0 );
        if( ! bReturn )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject: Far failed to find anything\n" );
        }
        else if( closest != 1.0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject: Far failed for float, got %f\n", closest );
        }
    }
    
    {
        CNearTree<double> tree;
        
        double f = 1.0;
        /* generate an unbalanced tree*/
        while( f > 0.0 && f >= DBL_MIN)
        {
            f /= 2.0;
        }
        
        f = DBL_MIN;
        while( f < 1.01 )
        {
            tree.insert( f );
            f *= 2.0;
        }
        
        double farthest = 0.0;
        const bool bReturnNear = tree.NearestNeighbor( 1000.0, farthest, 1000.0 );
        if( ! bReturnNear )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject Near failed to find anything for double\n" );
        }
        else if( farthest != 1.0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject Near failed for double, reverse tree, got %f\n", farthest );
        }
        
        double closest = 0.0;
        const bool bReturnFar = tree.FarthestNeighbor( closest, -11.0 );
        if( ! bReturnFar )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject Far failed to find anything for double\n" );
        }
        else if( closest != 1.0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindLastObject Far failed for double, reverse tree, got %f\n", closest );
        }
    }
}  // end testFindLastObject

/*=======================================================================*/
void testFindInSphereFromBottom( void )
{
    const int nmax = 100;
    CNearTree<double> tree;
    
    for( int i=1; i<=nmax; ++i )
    {
        tree.insert( (double)i );
    }
    
    /* generate an unbalanced tree*/
    for( int i=1; i<=nmax; ++i )
    {
        const double radius = 0.05 + (double)i;
        CNearTree<double> sphereReturn;
        const long lReturned = tree.FindInSphere( radius, sphereReturn, 0.9 );
        if( lReturned != (long)i )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testFindInSphereFromBottom for i=%d\n", i );
        }
    }
    for( int i=1; i<=nmax; ++i )
    {
        const double radius = 0.05 + (double)i;
        CNearTree<double> sphereReturn;
        const long lReturned = tree.LeftFindInSphere( radius, sphereReturn, 0.9 );
        if( lReturned != (long)i )
        {
            ++g_errorCount;
            fprintf(stdout, "LeftFindInSphere failed in testFindInSphereFromBottom for i=%d\n", i );
        }
    }
    
}  //  end testFindInSphereFromBottom

/*=======================================================================*/
void testFindInSphereFromTop( void )
{
    const int nmax = 100;
    CNearTree<double> tree;
    
    for( int i=1; i<=nmax; ++i )
    {
        tree.insert( (double)i );
    }
    
    /* generate an unbalanced tree*/
    for( int i=1; i<=nmax; ++i )
    {
        const double radius = 0.05 + (double)i;
        CNearTree<double> sphereReturn;
        const long lReturned = tree.FindInSphere( radius, sphereReturn, 0.1 + (double)nmax );
        if( lReturned != (long)i )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testFindInSphereFromTop for i=%d\n", i );
        }
    }
    
    std::vector<double> v;
    /* generate an unbalanced tree*/
    for( int i=1; i<=nmax; ++i )
    {
        v.clear( );
        const double radius = 0.05 + (double)i;
        CNearTree<double> sphereReturn;
        const long lReturned = tree.FindInSphere( radius, v, 0.1 + (double)nmax );
        if( lReturned != (long)i )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed for vector in testFindInSphereFromTop for i=%d\n", i );
        }
    }
}  // end testFindInSphereFromTop

/*=======================================================================*/
void testOutSphere( void )
{
    const int nmax = 100;
    CNearTree<double> tree;
    
    /* generate an unbalanced tree*/
    for( int i=1; i<=nmax; ++i )
    {
        tree.insert( (double)i );
    }
    
    // now test the NearTree return
    for( int i=1; i<=nmax; ++i )
    {
        CNearTree<double> sphereReturn;
        const double radius = 0.05 + (double)i;
        const long lReturned = tree.FindOutSphere( radius, sphereReturn, 0.1 + (double)nmax );
        if( lReturned != 100-(long)i )
        {
            ++g_errorCount;
            fprintf(stdout, "testOutSphere: FindOutSphere failed for i=%d\n", i );
        }
    }
    
    // now test the vector return
    for( int i=1; i<=nmax; ++i )
    {
        std::vector<double> v;
        v.clear( );
        const double radius = 0.05 + (double)i;
        const long lReturned = tree.FindOutSphere( radius, v, 0.1 + (double)nmax );
        if( lReturned != 100-(long)i )
        {
            ++g_errorCount;
            fprintf(stdout, "testOutSphere: FindOutSphere failed for for vector for i=%d\n", i );
        }
    }
}  // end testOutSphere

/*
 Test CNearTree with a bunch of random numbers (integers).
 */
/*=======================================================================*/
void testRandomTree1( const int nRequestedRandoms )
{
    const int n = nRequestedRandoms;
    CNearTree<int> tree;
    int nmax = INT_MIN;
    int nmin = INT_MAX;
    
    /* Build the tree with n random numbers. Remember the largest and smallest values. */
    for( int i=0; i<n; ++i )
    {
        const int next = (int)(rhr.urand()*((double)RHrand::RHRAND_MAX));
        tree.insert( next );
        if( next > nmax ) nmax = next;
        if( next < nmin ) nmin = next;
    }
    
    {
        /*verify that the correct extremal point is detected (from below)*/
        int closest=INT_MAX;
        const bool bNear = tree.NearestNeighbor( (double)LONG_MAX, closest, INT_MIN/2 );
        if( ! bNear )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree1: NearestNeighbor failed to find anything\n" );
        }
        else if( closest != nmin )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree1: NearestNeighbor failed for min\n" );
        }
        
        int farthest=INT_MIN;
        const bool bFar = tree.FarthestNeighbor( farthest, INT_MIN/2 );
        if( !bFar )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree1: FarthestNeighbor failed to find anything\n" );
        }
        else if( farthest != nmax )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree1: FarthestNeighbor failed in testRandomTree1 for min\n" );
        }
    }
    
    {
        /*verify that the correct extremal point is detected (from above)*/
        int closest=INT_MIN;
        const bool bNear = tree.NearestNeighbor( (double)LONG_MAX, closest, INT_MAX/2 );
        if( ! bNear )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree1: NearestNeighbor failed to find anything for max\n" );
        }
        else if( closest != nmax )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree1: NearestNeighbor failed for max\n" );
        }
        
        int farthest;
        const bool bFar = tree.FarthestNeighbor( farthest, INT_MAX/2 );
        if( !bFar )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree1: FarthestNeighbor failed to find anything for min\n" );
        }
        else if( farthest != nmin )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree1: FarthestNeighbor failed for min\n" );
        }
    }
    
    {
        /*verify that for very large radius, every point is detected (from below)*/
        std::vector<int> v;
        const double radius = DBL_MAX;
        CNearTree<int> sphereReturn;
        const long lReturn = tree.FindInSphere( radius, sphereReturn, INT_MIN/2 );
        if( lReturn != n )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testRandomTree1, n=%d, lReturn=%ld\n", n, lReturn );
        }
    }

    {
        /*verify that for very large radius, every point is detected (from below)*/
        std::vector<int> v;
        const double radius = DBL_MAX;
        CNearTree<int> sphereReturn;
        std::vector<size_t> sphereIndices;
        const long lReturn = tree.FindInSphere( radius, sphereReturn, sphereIndices, INT_MIN/2 );
        if( lReturn != n || sphereIndices.size() != (size_t)n )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testRandomTree1, n=%d, lReturn=%ld, indices=%ld\n", 
                    n, (long)lReturn, (long)sphereIndices.size() );
        } else {
            for (size_t ii = 0; ii < (size_t) n; ii++) {
                if (tree[sphereIndices[ii]]!=sphereReturn[ii]) {
                    ++g_errorCount;
                    fprintf(stdout, "FindInSphere: testRandomTree1: tree[%ld] != sphereReturn[%ld]\n",
                            (long)sphereIndices[ii], (long)ii );
                }
            }
        }
    }
    
    {
        /*verify that we find NO points if we are below the lowest and with too small radius*/
        const double radius = .5;
        CNearTree<int> sphereReturn;
        const long lReturn = tree.FindInSphere( radius, sphereReturn, nmin-1 );
        if( lReturn != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testRandomTree1 found points incorrectly, n=%d, lReturn=%ld\n", n, lReturn );
        }
    }

    {
        /*verify that we find NO points if we are below the lowest and with too small radius*/
        const double radius = .5;
        CNearTree<int> sphereReturn;
        CNearTree<size_t> sphereIndices;
        const long lReturn = tree.FindInSphere( radius, sphereReturn, nmin-1 );
        if( lReturn != 0 || sphereIndices.size() !=0)
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testRandomTree1 found points or indices incorrectly, n=%d, lReturn=%ld, indices=%ld\n",
                    n, lReturn, (long)sphereIndices.size());
        }
    }
    
    {
        /*test that the number of found points in a sphere is non-deceasing with increasing radius*/
        long lReturn = 0;
        int lastFoundCount = 0;
        
        double radius = 0.00001; /* start with a very small radius (remember these are int's) */
        while( radius < (double)(5*(nmax-nmin)) )
        {
            CNearTree<int> sphereReturn;
            lReturn = tree.FindInSphere( radius, sphereReturn, nmin-1 );
            if( lReturn < lastFoundCount )
            {
                ++g_errorCount;
                fprintf(stdout, "FindInSphere in testRandomTree1 found DECREASING count on increasing radius for radius=%f\n", radius );
                break;
            }
            else
            {
                lastFoundCount = lReturn;
                radius *= 1.414;
            }
        }
        
        /* verify that all points were included in the final check (beyond all reasonable distances) */
        if( lReturn != n )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere in testRandomTree1 did not find all the points\n" );
        }
    }
} // end testRandomTree1

/*
 Test CNearTree with a bunch of random numbers (integers).
 */
/*=======================================================================*/
void testRandomTree2( const int nRequestedRandoms )
{
    const int n = nRequestedRandoms;
    CNearTree<int, double, -11> tree;
    int nmax = INT_MIN;
    int nmin = INT_MAX;
    
    /* Build the tree with n random numbers. Remember the largest and smallest values. */
    for( int i=0; i<n; ++i )
    {
        const int next = (int)(rhr.urand()*((double)RHrand::RHRAND_MAX));
        tree.insert( next );
        if( next > nmax ) nmax = next;
        if( next < nmin ) nmin = next;
    }
    
    {
        /*verify that the correct extremal point is detected (from below)*/
        int closest=INT_MAX;
        const bool bNear = tree.NearestNeighbor( (double)LONG_MAX, closest, INT_MIN/2 );
        if( ! bNear )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree2: NearestNeighbor failed to find anything\n" );
        }
        else if( closest != nmin )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree2: NearestNeighbor failed for min\n" );
        }
        
        int farthest=INT_MIN;
        const bool bFar = tree.FarthestNeighbor( farthest, INT_MIN/2 );
        if( !bFar )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree2: FarthestNeighbor failed to find anything\n" );
        }
        else if( farthest != nmax )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree2: FarthestNeighbor failed in testRandomTree2 for min\n" );
        }
    }
    
    {
        /*verify that the correct extremal point is detected (from above)*/
        int closest=INT_MIN;
        const bool bNear = tree.NearestNeighbor( (double)LONG_MAX, closest, INT_MAX/2 );
        if( ! bNear )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree2: NearestNeighbor failed to find anything for max\n" );
        }
        else if( closest != nmax )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree2: NearestNeighbor failed for max\n" );
        }
        
        int farthest;
        const bool bFar = tree.FarthestNeighbor( farthest, INT_MAX/2 );
        if( !bFar )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree2: FarthestNeighbor failed to find anything for min\n" );
        }
        else if( farthest != nmin )
        {
            ++g_errorCount;
            fprintf(stdout, "testRandomTree2: FarthestNeighbor failed for min\n" );
        }
    }
    
    {
        /*verify that for very large radius, every point is detected (from below)*/
        std::vector<int> v;
        const double radius = DBL_MAX;
        CNearTree<int,double,-11> sphereReturn;
        const long lReturn = tree.FindInSphere( radius, sphereReturn, INT_MIN/2 );
        if( lReturn != n )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testRandomTree2, n=%d, lReturn=%ld\n", n, lReturn );
        }
    }

    {
        /*verify that for very large radius, every point is detected (from below)*/
        std::vector<int> v;
        const double radius = DBL_MAX;
        CNearTree<int,double,-11> sphereReturn;
        std::vector<size_t> sphereIndices;
        const long lReturn = tree.FindInSphere( radius, sphereReturn, sphereIndices, INT_MIN/2 );
        if( lReturn != n || sphereIndices.size() != (size_t)n )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testRandomTree2,  n=%d, lReturn=%ld, indices=%ld\n", 
                    n, (long)lReturn, (long)sphereIndices.size() );
        } else {
            for (size_t ii = 0; ii < (size_t)n; ii++) {
                if (tree[sphereIndices[ii]]!=sphereReturn[ii]) {
                    ++g_errorCount;
                    fprintf(stdout, "FindInSphere: testRandomTree2: tree[%ld] != sphereReturn[%ld]\n",
                            (long)sphereIndices[ii], (long)ii );
                }
            }
        }
    }
    
    {
        /*verify that we find NO points if we are below the lowest and with too small radius*/
        const double radius = .5;
        CNearTree<int,double,-11> sphereReturn;
        const long lReturn = tree.FindInSphere( radius, sphereReturn, nmin-1 );
        if( lReturn != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testRandomTree2 found points incorrectly, n=%d, lReturn=%ld\n", n, lReturn );
        }
    }
    {
        /*verify that we find NO points if we are below the lowest and with too small radius*/
        const double radius = .5;
        CNearTree<int,double,-11> sphereReturn;
        std::vector<size_t> sphereIndices;
        const long lReturn = tree.FindInSphere( radius, sphereReturn, sphereIndices, nmin-1 );
        if( lReturn != 0 || sphereIndices.size() !=0 )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere failed in testRandomTree2 found points or indices incorrectly, n=%d, lReturn=%ld, indices=%ld\n",
                    n, lReturn, (long)sphereIndices.size());
        }
    }
    
    {
        /*test that the number of found points in a sphere is non-deceasing with increasing radius*/
        long lReturn = 0;
        int lastFoundCount = 0;
        
        double radius = 0.00001; /* start with a very small radius (remember these are int's) */
        while( radius < (double)(5*(nmax-nmin)) )
        {
            CNearTree<int,double,-11> sphereReturn;
            lReturn = tree.FindInSphere( radius, sphereReturn, nmin-1 );
            if( lReturn < lastFoundCount )
            {
                ++g_errorCount;
                fprintf(stdout, "FindInSphere in testRandomTree2 found DECREASING count on increasing radius for radius=%f\n", radius );
                break;
            }
            else
            {
                lastFoundCount = lReturn;
                radius *= 1.414;
            }
        }

        lastFoundCount = 0;
        radius = 0.00001; /* start with a very small radius (remember these are int's) */
        while( radius < (double)(5*(nmax-nmin)) )
        {
            CNearTree<int,double,-11> sphereReturn;
            std::vector<size_t> sphereIndices;
            lReturn = tree.FindInSphere( radius, sphereReturn, sphereIndices, nmin-1 );
            if( lReturn < lastFoundCount || sphereIndices.size() != (size_t)lReturn )
            {
                ++g_errorCount;
                fprintf(stdout, "FindInSphere in testRandomTree2 found DECREASING count on increasing radius for radius=%f"
                        " or mismatched index count %ld != %ld\n",
                        radius, (long)sphereIndices.size(), (long)lastFoundCount );
                break;
            }
            else
            {
                for (size_t ii = 0; ii < sphereIndices.size(); ii++) {
                    if (tree[sphereIndices[ii]]!=sphereReturn[ii]) {
                        ++g_errorCount;
                        fprintf(stdout, "FindInSphere in testRandomTree2 mismatch between tree[%ld] and sphereReturn[%ld]\n",
                          (long)sphereIndices[ii], (long)ii);
                    }
                }
                lastFoundCount = lReturn;
                radius *= 1.414;
            }
        }
        
        /* verify that all points were included in the final check (beyond all reasonable distances) */
        if( lReturn != n )
        {
            ++g_errorCount;
            fprintf(stdout, "FindInSphere in testRandomTree2 did not find all the points\n" );
        }
    }
}  // end testRandomTree2

/*=======================================================================*/
/* make a 17-dimension vector class for testing */
class vec17
    {
    public:
        double pd[17];
        int dim;
        double length;  // just for a signature for debugging
        vec17( ) :
        dim(17), length(0)
        {
            for( int i=0; i<dim; ++i )
            {
                pd[i] = rhr.urand()*((double)RHrand::RHRAND_MAX);
            }
            length = Norm( );
        }
        explicit vec17( const double d ):
        dim(17), length(0)
        {
            for( int i=0; i<dim; ++i )
            {
                pd[i] = d;
            }
            length = Norm( );
        }
        explicit vec17( const int n ):
        dim(17), length(0)
        {
            for( int i=0; i<dim; ++i )
            {
                pd[i] = double(n);
            }
            length = Norm( );
        }
        ~vec17( void )
        {
        } 
        
        vec17 operator-( const vec17& v ) const /* USERS: be sure to make both const */
        {
            vec17 vtemp;
            for( int i=0; i<dim; ++i )
            {
                vtemp.pd[i] = this->pd[i] - v.pd[i];
            }
            vtemp.length = Norm(  );
            return( vtemp );
        }
        double Norm( void ) const
        {
            double dtemp = 0.0;
            for( int i=0; i<dim; ++i )
            {
                dtemp += pd[i]*pd[i];  //  L2 measure here
            }
            return( double(sqrt( dtemp )) );
        }
        
        vec17& operator-= ( const vec17 ) { return ( *this ); }; // just to keep LINT happy
    };  // end vec17


/*=======================================================================*/
void testBigVector(  )
{
    const int vectorsize = 1000;   
    CNearTree<vec17> tree;
    vec17 vAll[vectorsize]; /* keep a list of all of the input so we can find particular entries */
    double rmax = -DBL_MAX;
    double rmin =  DBL_MAX;
    vec17 v17min; /* to be the point nearest to the origin */
    vec17 v17max; /* to be the point farthest from the origin */
    
    /* All of the coordinate values will be in the range 0-RHrand::RHRAND_MAX. In other words,
     all of the data points will be within a 17-D cube that has a corner at
     the origin of the space.
     */
    for( int i=0; i<vectorsize; ++i )
    {
        vec17 v;
        if( v.Norm( ) < rmin )
        {
            rmin = v.Norm( );
            v17min = v;
        }
        
        if( v.Norm( ) > rmax )
        {
            rmax = v.Norm( );
            v17max = v;
        }
        
        tree.insert( v );
        vAll[i] = v;
    }
    
    timetreecommand(tree,"FarthestNeighbor",{
                    {
                    size_t estdim = (size_t)(0.5+tree.GetDimEstimate());
                    if ( estdim < 6) {
                    ++g_errorCount;   
                    fprintf(stdout, "testBigVector: dimension estimate %ld < 6 \n",(long)estdim);
                    } else {
                    fprintf(stdout, "testBigVector: dimension estimate %ld\n",(long)estdim);
                    }
                    
                    {
                    /* Find the point farthest from the point that was nearest the origin. */
                    vec17 vFarthest;
                    tree.FarthestNeighbor( vFarthest, v17min );
                    
                    /* Brute force search for the farthest */
                    vec17 vSearch;
                    double dmax = -DBL_MAX;
                    for( int i=0; i<vectorsize; ++i )
                    {
                    if( ( vAll[i] - v17min ).Norm( ) > dmax )
                    {
                    dmax = ( vAll[i] - v17min ).Norm( );
                    vSearch = vAll[i];
                    }
                    }
                    
                    double distdiff = ( (vSearch-v17min) - (vFarthest-v17min) ).Norm( );
                    distdiff = (distdiff<0)?-distdiff:distdiff;
                    if( distdiff > DBL_MIN )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "in testBigVector, apparently FarthestNeighbor has failed\n" );
                    }
                    }
                    
                    {
                    /* somewhere in the middle, find a point and its nearest neighbor */
                    /* make sure that each includes the other in sphere search */
                    
                    const vec17 vBox17Center( (double)(RHrand::RHRAND_MAX/2) );
                    vec17 vNearCenter;
                    vec17 vCloseToNearCenter;
                    tree.NearestNeighbor( double(RHrand::RHRAND_MAX/2)*sqrt(17.), vNearCenter, vBox17Center );
                    CNearTree<vec17> sphereReturn;
                    std::vector<size_t> sphereIndices;
                    unsigned long iFoundNearCenter = (unsigned long)tree.FindInSphere( double(RHrand::RHRAND_MAX/2)*sqrt(17.)/2., sphereReturn, vNearCenter );
                    
                    /* Brute force search for the point closest to the point closest to the center */
                    double dmin = DBL_MAX;
                    for( unsigned long i=0; i<iFoundNearCenter; ++i )
                    {
                    if( (vNearCenter - sphereReturn[i]).Norm( ) > DBL_MIN && ( vNearCenter-sphereReturn[i] ).Norm( ) < dmin )
                    {
                    dmin = ( vNearCenter-sphereReturn[i] ).Norm( );
                    vCloseToNearCenter = sphereReturn[i];
                    }
                    }
                    
                    {
                    //const double radius = ( vCloseToNearCenter - vNearCenter ).Norm( );
                    const double radius = RHrand::RHRAND_MAX*sqrt(17.0);
                    unsigned long iSphereFoundNearCenter = (unsigned long)tree.FindInSphere( radius, sphereReturn, vNearCenter );
                    
                    double searchRadius = radius/2;
                    double delta        = searchRadius;
                    int count = 0;
                    while( iSphereFoundNearCenter != 2 && count < 100 )
                    {
                    iSphereFoundNearCenter = (unsigned long)tree.FindInSphere( searchRadius, sphereReturn, vNearCenter );
                    if( iSphereFoundNearCenter > 2 )
                    {
                    searchRadius = searchRadius - delta/2;
                    }
                    else if( iSphereFoundNearCenter < 2 )
                    {
                    searchRadius = searchRadius + delta/2;
                    }
                    delta /= 2;
                    ++count;
                    }  // end while
                    
                    if( iSphereFoundNearCenter != 2 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: FindInSphere failed to find only 2 points\n" );
                    }
                    }
                    
                    {
                    //const double radius = ( vCloseToNearCenter - vNearCenter ).Norm( );
                    const double radius = RHrand::RHRAND_MAX*sqrt(17.0);
                    unsigned long iSphereFoundNearCenter = (unsigned long)tree.FindInSphere( radius, sphereReturn, vNearCenter );
                    
                    double searchRadius = radius/2;
                    double delta        = searchRadius;
                    int count = 0;
                    while( iSphereFoundNearCenter != 2 && count < 100 )
                    {
                    iSphereFoundNearCenter = (unsigned long)tree.FindInSphere( searchRadius, sphereReturn, sphereIndices, vNearCenter );
                    if ((size_t)iSphereFoundNearCenter != sphereIndices.size()) {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: FindInSphere mismatch size %ld with indices %ld\n",
                            (long)iSphereFoundNearCenter, (long)sphereIndices.size());
                    } else {
                    for (size_t ii = 0; ii < sphereIndices.size(); ii++) {
                    if ((tree[sphereIndices[ii]]-sphereReturn[ii]).Norm() != 0.) {
                    fprintf(stdout, "testBigVector: FindInSphere mismatch tree[%ld] != sphereReturn[%ld]\n",
                            (long)sphereIndices[ii], (long)ii);
                    }
                    }
                    }
                    if( iSphereFoundNearCenter > 2 )
                    {
                    searchRadius = searchRadius - delta/2;
                    }
                    else if( iSphereFoundNearCenter < 2 )
                    {
                    searchRadius = searchRadius + delta/2;
                    }
                    delta /= 2;
                    ++count;
                    }  // end while
                    
                    if( iSphereFoundNearCenter != 2 || sphereIndices.size() != 2 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: FindInSphere failed to find only 2 points or indices !=2\n" );
                    }
                    }
                    
                    if( dmin == DBL_MAX ) 
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: apparently FindInSphere failed\n" );
                    }
                    
                    {
                    /* Using zero radius, check that only one point is found when a point is searched
     with FindInSphere */
                    sphereReturn.clear( );
                    const long iFound = tree.FindInSphere( 0.0, sphereReturn, vNearCenter );
                    if( iFound < 1 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: FindInSphere found no points using zero radius\n" );
                    }
                    else if( iFound != 1 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: FindInSphere found more than %ld points using zero radius\n", iFound );
                    }
                    }
                    
                    {
                    /* Using minimal radius, check that at least 2 points are found when a point is searched
     with FindInSphere */
                    sphereReturn.clear( );
                    const long iFound = tree.FindInSphere( (vCloseToNearCenter-vNearCenter).Norm( ), sphereReturn, vNearCenter );
                    if( iFound < 2 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: FindInSphere found only 1 point\n" );
                    }
                    }
                    
                    {
                    /* Using small radius, check that only one point is found when a point is searched
     with FindInSphere */
                    sphereReturn.clear( );
                    const long iFound = tree.FindInSphere( (vCloseToNearCenter-vNearCenter).Norm( )*0.9, sphereReturn, vNearCenter );
                    if( iFound < 1 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: FindInSphere found no points using %f radius\n", (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
                    }
                    else if( iFound != 1 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: FindInSphere found %ld points using %f radius\n", iFound, (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
                    }
                    }
                    
                    }}
                    });
    
    timetreecommand(tree,"FarthestNeighbor Left",{
                    {size_t estdim = (size_t)(0.5+tree.GetDimEstimate());;
                    if ( estdim < 6) {
                    ++g_errorCount;   
                    fprintf(stdout, "testBigVector: dimension estimate %ld <6 \n",(long)estdim);
                    } else {
                    fprintf(stdout, "testBigVector: dimension estimate %ld\n",(long)estdim);
                    }
                    
                    {
                    /* Find the point farthest from the point that was nearest the origin. */
                    vec17 vFarthest;
                    tree.LeftFarthestNeighbor( vFarthest, v17min );
                    
                    /* Brute force search for the farthest */
                    vec17 vSearch;
                    double dmax = -DBL_MAX;
                    for( int i=0; i<vectorsize; ++i )
                    {
                    if( ( vAll[i] - v17min ).Norm( ) > dmax )
                    {
                    dmax = ( vAll[i] - v17min ).Norm( );
                    vSearch = vAll[i];
                    }
                    }
                    
                    double distdiff = ( (vSearch-v17min) - (vFarthest-v17min) ).Norm( );
                    distdiff = (distdiff<0)?-distdiff:distdiff;
                    if( distdiff > DBL_MIN )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "in testBigVector, apparently LeftFarthestNeighbor has failed\n" );
                    }
                    }
                    
                    {
                    /* somewhere in the middle, find a point and its nearest neighbor */
                    /* make sure that each includes the other in sphere search */
                    
                    const vec17 vBox17Center( (double)(RHrand::RHRAND_MAX/2) );
                    vec17 vNearCenter;
                    vec17 vCloseToNearCenter;
                    tree.LeftNearestNeighbor( double(RHrand::RHRAND_MAX/2)*sqrt(17.), vNearCenter, vBox17Center );
                    CNearTree<vec17> sphereReturn;
                    std::vector<size_t> sphereIndices;
                    unsigned long iFoundNearCenter = (unsigned long)tree.LeftFindInSphere( double(RHrand::RHRAND_MAX/2)*sqrt(17.)/2., sphereReturn, vNearCenter );
                    
                    /* Brute force search for the point closest to the point closest to the center */
                    double dmin = DBL_MAX;
                    for( unsigned long i=0; i<iFoundNearCenter; ++i )
                    {
                    if( (vNearCenter - sphereReturn[i]).Norm( ) > DBL_MIN && ( vNearCenter-sphereReturn[i] ).Norm( ) < dmin )
                    {
                    dmin = ( vNearCenter-sphereReturn[i] ).Norm( );
                    vCloseToNearCenter = sphereReturn[i];
                    }
                    }
                    
                    {
                    //const double radius = ( vCloseToNearCenter - vNearCenter ).Norm( );
                    const double radius = RHrand::RHRAND_MAX*sqrt(17.0);
                    unsigned long iSphereFoundNearCenter = (unsigned long)tree.LeftFindInSphere( radius, sphereReturn, vNearCenter );
                    
                    double searchRadius = radius/2;
                    double delta        = searchRadius;
                    int count = 0;
                    while( iSphereFoundNearCenter != 2 && count < 100 )
                    {
                    iSphereFoundNearCenter = (unsigned long)tree.LeftFindInSphere( searchRadius, sphereReturn, vNearCenter );
                    if( iSphereFoundNearCenter > 2 )
                    {
                    searchRadius = searchRadius - delta/2;
                    }
                    else if( iSphereFoundNearCenter < 2 )
                    {
                    searchRadius = searchRadius + delta/2;
                    }
                    delta /= 2;
                    ++count;
                    }  // end while
                    
                    if( iSphereFoundNearCenter != 2 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: LeftFindInSphere failed to find only 2 points\n" );
                    }
                    }
                    
                    {
                    //const double radius = ( vCloseToNearCenter - vNearCenter ).Norm( );
                    const double radius = RHrand::RHRAND_MAX*sqrt(17.0);
                    unsigned long iSphereFoundNearCenter = (unsigned long)tree.LeftFindInSphere( radius, sphereReturn, vNearCenter );
                    
                    double searchRadius = radius/2;
                    double delta        = searchRadius;
                    int count = 0;
                    while( iSphereFoundNearCenter != 2 && count < 100 )
                    {
                    iSphereFoundNearCenter = (unsigned long)tree.LeftFindInSphere( searchRadius, sphereReturn, sphereIndices, vNearCenter );
                    if ((size_t)iSphereFoundNearCenter != sphereIndices.size()) {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: LeftFindInSphere mismatch size %ld with indices %ld\n",
                            (long)iSphereFoundNearCenter, (long)sphereIndices.size());
                    } else {
                    for (size_t ii = 0; ii < sphereIndices.size(); ii++) {
                    if ((tree[sphereIndices[ii]]-sphereReturn[ii]).Norm() != 0.) {
                    fprintf(stdout, "testBigVector: LeftFindInSphere mismatch tree[%ld] != sphereReturn[%ld]\n",
                            (long)sphereIndices[ii], (long)ii);
                    }
                    }
                    }
                    if( iSphereFoundNearCenter > 2 )
                    {
                    searchRadius = searchRadius - delta/2;
                    }
                    else if( iSphereFoundNearCenter < 2 )
                    {
                    searchRadius = searchRadius + delta/2;
                    }
                    delta /= 2;
                    ++count;
                    }  // end while
                    
                    if( iSphereFoundNearCenter != 2 || sphereIndices.size() != 2 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: Left FindInSphere failed to find only 2 points or indices !=2\n" );
                    }
                    }
                    
                    if( dmin == DBL_MAX ) 
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: apparently LeftFindInSphere failed\n" );
                    }
                    
                    {
                    /* Using zero radius, check that only one point is found when a point is searched
     with FindInSphere */
                    sphereReturn.clear( );
                    const long iFound = tree.LeftFindInSphere( 0.0, sphereReturn, vNearCenter );
                    if( iFound < 1 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: LeftFindInSphere found no points using zero radius\n" );
                    }
                    else if( iFound != 1 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: LeftFindInSphere found more than %ld points using zero radius\n", iFound );
                    }
                    }
                    
                    {
                    /* Using minimal radius, check that at least 2 points are found when a point is searched
     with FindInSphere */
                    sphereReturn.clear( );
                    const long iFound = tree.LeftFindInSphere( (1.+DBL_EPSILON)*(vCloseToNearCenter-vNearCenter).Norm( ), sphereReturn, vNearCenter );
                    if( iFound < 2 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: LeftFindInSphere found only 1 point\n" );
                    }
                    }
                    
                    {
                    /* Using small radius, check that only one point is found when a point is searched
     with FindInSphere */
                    sphereReturn.clear( );
                    const long iFound = tree.LeftFindInSphere( (vCloseToNearCenter-vNearCenter).Norm( )*0.9, sphereReturn, vNearCenter );
                    if( iFound < 1 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: LeftFindInSphere found no points using %f radius\n", (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
                    }
                    else if( iFound != 1 )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "testBigVector: LeftFindInSphere found %ld points using %f radius\n", iFound, (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
                    }
                    }
                    
                    }}
                    });        
    
}  // testBigVector

/*=======================================================================*/
void testBackwardForward( void )
{
    CNearTree<double> tree;
    const int nMax = 1000;
    
    for( int i=0; i<nMax; ++i )
    {
        tree.insert( (double)i );
        tree.insert( (double)(nMax-i) );
        tree.insert( (double)i + 0.25 );
        tree.insert( (double)(nMax-i) + 0.75 );
    }
    
    for( int i=100; i<300; ++i )
    {
        double closest;
        const bool bReturn = tree.NearestNeighbor( 1000.0, closest, (double)i+0.25 );
        if( ! bReturn )
        {
            ++g_errorCount;
            fprintf(stdout, "testBackwardForward: NearestNeighbor failed to find anything\n" );
        }
        else if( ABS( closest-((double)i+0.25) ) > DBL_MIN )
        {
            fprintf(stdout, "testBackwardForward::NearestNeighbor failed, closest=%f\n", closest );
        }
    }
}  // end testBackwardForward

/*=======================================================================*/

const long nmax = 10001;

void testDelayedInsertion( void )
{
    
    {
        // make sure that CompleteDelayedInsert flushes the delayed data
        CNearTree<double> tree;
        
        for( int i=1; i<=nmax; ++i )
        {
            const double insertValue = (double)(i);
            tree.insert( insertValue );
        }
        
        tree.CompleteDelayedInsert( );
        const size_t depth = tree.GetDepth( );
        const size_t insertedSize = tree.GetTotalSize( );
        const size_t delayed = tree.GetDeferredSize( );
        
        if( delayed != 0 || (long)insertedSize != nmax )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertion: CompleteDelayedInsert completion is incorrect\n" );
        }
        else if( depth >=nmax/2 )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertion: tree depth is too large, %lu is greater than %ld\n", (unsigned long)depth, nmax/2 );
        }
        fprintf(stdout, "testDelayedInsertionRandom: tree depth is  %ld\n", (unsigned long)depth );
#ifdef CNEARTREE_INSTRUMENTED
        if( tree.GetHeight( ) !=  tree.GetDepth( ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertionRandom: tree depth != tree height, %lu != %lu\n", (unsigned long)tree.GetDepth( ), (unsigned long)tree.GetHeight( ) );
        }
#endif
        
    }
    
    {
        // make sure that FindInSphere flushes the delayed data
        const long nmax2 = 100;
        CNearTree<double> tree;
        
        for( int i=1; i<=nmax2; ++i )
        {
            if( (i%2) == 0 )
            {
                tree.ImmediateInsert( (double)i );
            }
            else
            {
                tree.insert( (double)i );
            }
        }
        
        CNearTree<double> sphereReturn;
        const double radius = 1000.0;
        const long lReturned = tree.FindInSphere( radius, sphereReturn, 0.9 );
        
        if( lReturned != nmax2 )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertion: FindInSphere failed for nmax2=%ld, found %ld points\n", nmax2, lReturned );
        }
    }
    
    {
        // make sure that NearestNeighbor flushes the delayed data
        const long nmax2 = 100;
        CNearTree<double> tree;
        double fFinal = DBL_MAX;
        
        for( int i=1; i<=nmax2; ++i )
        {
            if( (i%2) == 0 && i!=nmax2) // ensure that the last one is delayed
            {
                tree.ImmediateInsert( (double)i );
            }
            else
            {
                tree.insert( (double)i );
                fFinal = (double)i;
            }
        }
        
        double closest;
        const double radius = 0.1;
        const bool bReturned = tree.NearestNeighbor( radius, closest, fFinal );
        
        if( ! bReturned )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertion: NearestNeighbor failed\n" );
        }
        else if( closest != fFinal )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertion: NearestNeighbor failed to find the data\n" );
        }
    }
    
    {
        // make sure that FarthestNeighbor flushes the delayed data
        const long nmax2 = 100;
        CNearTree<double> tree;
        double fFinal = DBL_MAX;
        
        for( int i=1; i<=nmax2; ++i )
        {
            if( (i%2) == 0 && i!=nmax2) // ensure that the last one is delayed
            {
                tree.ImmediateInsert( (double)i );
            }
            else
            {
                tree.insert( (double)i );
                fFinal = (double)i;
            }
        }
        
        double farthest;
        const bool bReturned = tree.FarthestNeighbor( farthest, -100.0 );
        
        if( ! bReturned )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertion: FarthestNeighbor failed\n" );
        }
        else if( farthest != fFinal )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertion: FarthestNeighbor failed to find the data\n" );
        }
    }
    
}  // end testDelayedInsertion

/*=======================================================================*/
void testDelayedInsertionRandom( void )
{
    
    {
        // make sure that CompleteDelayedInsert flushes the delayed data
        CNearTree<double> tree;
        
        for( int i=1; i<=nmax; ++i )
        {
            const double insertValue = (double)(i);
            tree.insert( insertValue );
        }
        
        tree.CompleteDelayedInsertRandom( );
        const size_t depth = tree.GetDepth( );
        const size_t insertedSize = tree.GetTotalSize( );
        const size_t delayed = tree.GetDeferredSize( );
        
        if( delayed != 0 || (long)insertedSize != nmax )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertionRandom: CompleteDelayedInsertRandom completion is incorrect\n" );
        }
        else if( depth >= sqrt( (double)(nmax/2) ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertionRandom: tree depth is too large, %lu is greater than %ld\n", (unsigned long)depth, (long)sqrt( (double)(nmax/2) ) );
        }
        fprintf(stdout, "testDelayedInsertionRandom: tree depth is  %ld\n", (unsigned long)depth );
#ifdef CNEARTREE_INSTRUMENTED
        if( tree.GetHeight( ) !=  tree.GetDepth( ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testDelayedInsertionRandom: tree depth != tree height, %lu != %lu\n", (unsigned long)tree.GetDepth( ), (unsigned long)tree.GetHeight( ) );
        }
#endif        
    }
    
}  // end testDelayedInsertionRandom

/*=======================================================================*/
void testIterators( void )
{
    CNearTree<int> tree;
    CNearTree<int>::iterator itEmpty = tree.back( );
    CNearTree<int>::iterator itTest  = tree.end( );
    if( itEmpty != tree.end( ) )
    {
        ++g_errorCount;
        fprintf(stdout, "testIterators: back failed for empty tree\n" );
    }
    
    if( itEmpty != tree.end( ) )
    {
        ++g_errorCount;
        fprintf(stdout, "testIterators: back failed for empty tree\n" );
    }
    
    const int nMax = 1000;
    
    for( int i=0; i<nMax; ++i )
    {
        tree.insert( (int)i );
        if( i == 1 )
        {
            const CNearTree<int>::iterator itSingle = tree.back( );
            if( (*itSingle) != 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testIterators: size failed for size=2 tree\n" );
            }
        }
    }
    
    {
        CNearTree<int>::iterator it;
        it = --tree.end( );
        if( *it != nMax-1 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test01 failed\n" );
        }
        
        CNearTree<int>::iterator itTest( it );
        if( itTest != it )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: copy construction failed\n" );
        }
        
        it = tree.begin( );
        if( *it != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test02 failed\n" );
        }
        
        it = it+1;
        if( *it != 1 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test03 failed\n" );
        }
        
        it = it-1;
        if( *it != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test04 failed\n" );
        }
        
        const int i0 = *it;
        if( i0 != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test05 failed\n" );
        }
        
        ++it;
        if( *it != 1 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test06 failed\n" );
        }
        
        --it;
        if( *it != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test07 failed\n" );
        }
        
        const CNearTree<int>::iterator itPlus = it++;
        if( *it != 1 )
        {
            *itPlus; // just to keep LINT happy
            ++g_errorCount;
            fprintf(stdout, "testIterators: test08 failed\n" );
        }
        
        it--;
        if( i0 != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test09 failed\n" );
        }
        
        ++it;
        if( *it != 1 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: test10 failed\n" );
        }
    }
    
    {
        CNearTree<int>::iterator it;
        
        const int searchValue = 14;
        const CNearTree<int>::iterator itEnd = tree.end( );
        it = tree.NearestNeighbor( 0.1, searchValue );
        if( it == itEnd )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: NearestNeighbor failed with iterator\n" );
        }
        else if( *it != searchValue )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: NearestNeighbor found wrong value with iterator, %d\n", *it );
        }
        
        it = tree.FarthestNeighbor( searchValue );
        if( it == tree.end( ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators:FarthestNeighbor failed with iterator\n" );
        }
        else if( *it != nMax-1 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: FarthestNeighbor found wrong value with iterator, %d\n", *it );
        }
    }
    
    {
        // test InSphere
        CNearTree<int> sphereTree;
        const double radius = 5.0;
        const long sizeReturned = tree.FindInSphere( radius, sphereTree, 50 );
        CNearTree<int>::iterator itSphere = sphereTree.begin( );
        const size_t sphereSizeIn = sphereTree.size( );
        
        if( (long)sphereSizeIn != sizeReturned )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: FindInSphere and size found different values, %ld and %ld\n", sizeReturned, (long)sphereSizeIn );
        }
        const long sizeReturned2In = tree.FindInSphere( radius, sphereTree, 50 );
        
        if( sizeReturned != sizeReturned2In )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: 2nd FindInSphere found different value, %ld and %ld\n", sizeReturned, sizeReturned2In );
        }
        
        std::vector<int> sphereVector;
        const long sizeReturnedV = tree.FindInSphere( radius, sphereVector, 50 );
        
        if( (long)sphereSizeIn != sizeReturnedV )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: FindInSphere for vector and size found different values, %ld and %ld\n", sizeReturned, (long)sphereSizeIn );
        }
        
        // test OutSphere
        const long sizeReturnedOut = tree.FindOutSphere( radius, sphereTree, 50 );
        itSphere = sphereTree.begin( );
        const size_t sphereSizeOut = sphereTree.size( );
        
        if( (long)sphereSizeOut != sizeReturnedOut )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: FindOutSphere and size found different values, %ld and %ld\n", sizeReturned, (long)sphereSizeOut );
        }
        const long sizeReturned2Out = tree.FindOutSphere( radius, sphereTree, 50 );
        
        if( sizeReturned2Out != sizeReturnedOut )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: 2nd FindOutSphere found different value, %ld and %ld\n", sizeReturned, sizeReturned2Out );
        }
        
        if( sizeReturned2In+sizeReturned2Out != nMax+2 )
        {
            // because there are 2 ints on the boundary, the count should be nMax+2
            ++g_errorCount;
            fprintf(stdout, "testIterators: total count from InSphere and OutSphere is wrong, %ld, should be%d\n",
                    sizeReturned2Out+sizeReturned2Out, nMax+2 );
        }
        
        sphereTree.insert( 999999 );
        CNearTree<int>::iterator it = sphereTree.end( );
        --it;
        
        if( *(it) != 999999 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: Error after incrementing iterator at end(1)\n" );
        }
        
        it = --sphereTree.end( );
        if( *(it) != 999999 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: Error after incrementing iterator at end(2)\n" );
        }
        
    }
    
    {
        vec17 v;
        CNearTree<vec17> sphereReturn;
        sphereReturn.insert( v );
        CNearTree<vec17>::iterator itv = sphereReturn.begin( );
        const int n = itv->dim;
        
        if( n != 17 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: operator-> got wrong value for dim, %d\n", n );
        }
        const double d = itv->pd[0];
        
        if( d != v.pd[0] )
        {
            ++g_errorCount;
            fprintf(stdout, "testIterators: operator-> got wrong value for pd[0], %g\n", d );
        }
    }
}  // end testIterators

/*=======================================================================*/
class v2
    {
        int m,n;
    public:
        v2( const int mm, const int nn ) :
        m(mm), n(nn)
        {
        }
        
        v2 operator- ( const v2& v ) const
        {
            return( v2( m-v.m, n-v.n ) );
        }
        double Norm( void ) const
        {
            return( sqrt( (double)( n*n + m*m ) ) );  //  L2 measure
        }
        
    }; // end of v2



/*=======================================================================*/
void testFindInAnnulus( void )
{
    {
        CNearTree<int> tree;
        CNearTree<int>::iterator itEmpty = tree.back( ); // make sure back works
        itEmpty = tree.back( );
        
        const int nMax = 1000;
        
        for( int i=0; i<nMax; ++i )
        {
            tree.insert( (int)i );
        }
        const double r1 = 100.1;
        const double r2 = 299.9;
        
        {
            
            CNearTree<int> annulusTree;
            const long lInAnnulus = tree.FindInAnnulus( r1, r2, annulusTree, 0 );
            
            if( lInAnnulus != (299-101+1) )
            {
                ++g_errorCount;
                fprintf(stdout, "testFindInAnnulus: wrong number of objects found\n" );
            }
            
            CNearTree<int>::iterator itNear = annulusTree.NearestNeighbor( 1000.0, 0 );
            
            if( *itNear != 101 )
            {
                ++g_errorCount;
                fprintf(stdout, "testFindInAnnulus: lowest value (%d) is incorrect\n", *itNear );
            }
            
            CNearTree<int>::iterator itFar = annulusTree.FarthestNeighbor( 0 );
            if( *itFar != 299 )
            {
                ++g_errorCount;
                fprintf(stdout, "testFindInAnnulus: highest value (%d) is incorrect\n", *itFar );
            }
        }

        {
            
            CNearTree<int> annulusTree;
            std::vector<size_t> annulusIndices;
            const long lInAnnulus = tree.FindInAnnulus( r1, r2, annulusTree, annulusIndices, 0 );
            
            if( lInAnnulus != (299-101+1) || annulusIndices.size() != (299-101+1) )
            {
                ++g_errorCount;
                fprintf(stdout, "testFindInAnnulus: wrong number of objects found\n" );
            } else {
                for (size_t ii=0; ii < annulusIndices.size(); ii++) {
                    if (tree[annulusIndices[ii]] != annulusTree[ii]) {
                        ++g_errorCount;
                        fprintf(stdout, "testFindInAnnulus: mismatch tree[%ld] != annulusTree[%ld]\n",
                                (long)annulusIndices[ii], (long)ii);                        
                    }
                }
            }
            
            CNearTree<int>::iterator itNear = annulusTree.NearestNeighbor( 1000.0, 0 );
            
            if( *itNear != 101 )
            {
                ++g_errorCount;
                fprintf(stdout, "testFindInAnnulus: lowest value (%d) is incorrect\n", *itNear );
            }
            
            CNearTree<int>::iterator itFar = annulusTree.FarthestNeighbor( 0 );
            if( *itFar != 299 )
            {
                ++g_errorCount;
                fprintf(stdout, "testFindInAnnulus: highest value (%d) is incorrect\n", *itFar );
            }
        }
        
        {
            std::vector<int> annulusvector;
            const long lInAnnulus = tree.FindInAnnulus( r1, r2, annulusvector, 0 );
            
            if( lInAnnulus != (299-101+1) )
            {
                ++g_errorCount;
                fprintf(stdout, "testFindInAnnulus: for vector, wrong number of objects found\n" );
            }
        }
    }
    {
        // test annulus in the case of more than 1-D using CNearTree return
        CNearTree< v2 > tree;
        
        for( int i=0; i<30; ++i )
        {
            for( int j=0; j<30; ++j )
            {
                tree.insert( v2(i,j) );
            }
        }
        
        double estdim = tree.GetDimEstimate()+tree.GetDimEstimateEsd();
        if ( estdim <= 1.5) {
            ++g_errorCount;   
            fprintf(stdout, "testFindInAnnulus: dimension estimate %g <= 1.5\n",estdim);
        } else {
            fprintf(stdout, "testFindInAnnulus: dimension estimate %g\n",estdim);
        }
        
        
        const double r1 = 4.0;
        const double r2 = 8.0;
        
        const v2 probe( -1, -1 );
        
        // use FindInAnnulus to find the points in the annulus
        CNearTree< v2 > annulusResult;
        const long searchFoundInAnnulus = tree.FindInAnnulus( r2, r1, annulusResult, probe );
        
        // now count all the points in the annulus by brute force
        
        int count = 0;
        const double radius = 0.001; // specify a very small radius so integer grid will only detect single points
        for( int i=0; i<30; ++i )
        {
            for( int j=0; j<30; ++j )
            {
                const v2 probe2( i, j );
                v2 vReturn( 0, 0 ); // dummy coords for constructor
                if( annulusResult.NearestNeighbor( radius, vReturn, probe2 ) )
                {
                    ++count;
                }
            }
        }
        
        if( searchFoundInAnnulus != count )
        {
            ++g_errorCount;
            fprintf(stdout, "testFindInAnnulus: wrong number of points found in annulus by NearTree, %ld, direct count=%d\n", searchFoundInAnnulus, count );
        }
    }
    
}  // end testFindInAnnulus

/*=======================================================================*/
void testMisc( void )
{
    CNearTree<int, double, -3> nt;
    for( int i=0; i<20; ++i )
    {
        nt.insert( i );
    }
    
    const std::vector<int> v = nt;
    if( v.size( ) != nt.size( ) )
    {
        ++g_errorCount;
        fprintf(stdout, "testMisc: wrong number of points %ld in vector\n", (long)v.size( ) );
    }
    
    CNearTree<int, double, -3> nt1;
    nt1 = nt;
    if( nt1.size( ) != nt.size( ) )
    {
        ++g_errorCount;
        fprintf(stdout, "testMisc: wrong number of points %ld in CNearTree\n", (long)nt1.size( ) );
    }
    
    const size_t indexToRetrieve = 3;
    if( nt.at( indexToRetrieve ) != (int)indexToRetrieve )
    {
        ++g_errorCount;
        fprintf(stdout, "testMisc: 'at' returned wrong value, %d\n",nt.at( 3 ) );
    }
}  //  end testMisc

/*=======================================================================*/
void testIntegerReturn( void )
{
    {
        CNearTree<int, int> nt;
        
        const int nmax2 = 20;
        for( int i=0; i<nmax2; ++i )
        {
            nt.insert( i );
        }
        
        int closest=-1;
        if( ! nt.NearestNeighbor( 1000, closest, 100 ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testIntegerReturn: NearestNeighbor failed\n" );
        }
        else if( closest != nmax2-1 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIntegerReturn: NearestNeighbor return wrong value, %d\n", closest );
        }
    }
    
    {
        CNearTree<int, long> nt;
        
        const int nmax2 = 20;
        for( int i=-nmax2; i<nmax2; ++i )
        {
            nt.insert( i );
        }
        
        int closest=-nmax2-1;
        if( ! nt.NearestNeighbor( 1000, closest, 100 ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testIntegerReturn: NearestNeighbor failed\n" );
        }
        else if( closest != nmax2-1 )
        {
            ++g_errorCount;
            fprintf(stdout, "testIntegerReturn: NearestNeighbor return wrong value, %d\n", closest );
        }
    }
}  // end testIntegerReturn


/*=======================================================================*/
/* make a 17-dimension all-integer vector class for testing */
class intVec17
    {
    public:
        int pd[17];
        int dim;
        double length;  // just for a signature for debugging
        intVec17( ) :
        dim(17),
        length( 0.0 )
        {
            for( int i=0; i<dim; ++i )
            {
                pd[i] = (int)(rhr.urand()*((double)RHrand::RHRAND_MAX));
            }
            length = this->Norm( );
        };
        
        explicit intVec17( const int d ) :
        dim( 17 ),
        length( 0.0 )
        {
            for( int i=0; i<dim; ++i )
            {
                pd[i] = d;
            }
            length = this->Norm( );
        };
        
        ~intVec17( void )
        {
        };
        
        int Norm( void ) const
        {
            long dtemp = 0;
            for( int i=0; i<dim; ++i )
            {
                dtemp += ABS(pd[i]);  // L1 measure here
            }
            return( dtemp );
        };
        
        intVec17 operator-( const intVec17& v ) const /* USERS: be sure to make both const */
        {
            intVec17 vtemp;
            for( int i=0; i<dim; ++i )
            {
                vtemp.pd[i] = pd[i] -v.pd[i];
            }
            vtemp.length = vtemp.Norm( );
            return( vtemp );
        };
        
        intVec17& operator-= ( const intVec17 ) { return ( *this ); }; // just to keep LINT happy
    }; // intVec17

/*=======================================================================*/

void testBigIntVec( void )
{
    const int vectorsize = 1000;   
    CNearTree<intVec17, int, -2> tree;
    intVec17 vAll[vectorsize]; /* keep a list of all of the input so we can find particular entries */
    double rmax = -DBL_MAX;
    double rmin =  DBL_MAX;
    intVec17 v17min; /* to be the point nearest to the origin */
    intVec17 v17max; /* to be the point farthest from the origin */
    
    /* All of the coordinate values will be in the range 0-RHrand::RHRAND_MAX. In other words,
     all of the data points will be within a 17-D cube that has a corner at
     the origin of the space.
     */
    for( int i=0; i<vectorsize; ++i )
    {
        intVec17 v;
        if( v.Norm( ) < rmin )
        {
            rmin = v.Norm( );
            v17min = v;
        }
        
        if( v.Norm( ) > rmax )
        {
            rmax = v.Norm( );
            v17max = v;
        }
        
        tree.insert( v );
        vAll[i] = v;
    }
    
    double estdim = tree.GetDimEstimate(0.01)+tree.GetDimEstimateEsd();
    if ( estdim < 3.5) {
        ++g_errorCount;   
        fprintf(stdout, "testBigIntVec: dimension estimate %g < 3.5\n",estdim);
    } else { 
        fprintf(stdout, "testBigIntVec: dimension estimate %g\n",estdim);
    }
            
    timetreecommand(tree, "testBigIntVector",{
                    {
                    /* Find the point farthest from the point that was nearest the origin. */
                    intVec17 vFarthest;
                    tree.FarthestNeighbor( vFarthest, v17min );
                    
                    /* Brute force search for the farthest */
                    intVec17 vSearch;
                    double dmax = -DBL_MAX;
                    for( int i=0; i<vectorsize; ++i )
                    {
                    if( ( vAll[i] - v17min ).Norm( ) > dmax )
                    {
                    dmax = ( vAll[i] - v17min ).Norm( );
                    vSearch = vAll[i];
                    }
                    }
                    
                    int distdiff = (int)((vSearch-v17min).Norm( )-(vFarthest-v17min).Norm( ));
                    distdiff = (distdiff<0)?-distdiff:distdiff;
                    if( distdiff > DBL_MIN )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "in testBigIntVector, apparently FarthestNeighbor has failed\n" );
                    }
                    }
                    });
    
    timetreecommand(tree, "testBigIntVector Left",{
                    {
                    /* Find the point farthest from the point that was nearest the origin. */
                    intVec17 vFarthest;
                    tree.LeftFarthestNeighbor( vFarthest, v17min );
                    
                    /* Brute force search for the farthest */
                    intVec17 vSearch;
                    double dmax = -DBL_MAX;
                    for( int i=0; i<vectorsize; ++i )
                    {
                    if( ( vAll[i] - v17min ).Norm( ) > dmax )
                    {
                    dmax = ( vAll[i] - v17min ).Norm( );
                    vSearch = vAll[i];
                    }
                    }
                    
                    int distdiff = (int)((vSearch-v17min).Norm( )-(vFarthest-v17min).Norm( ));
                    distdiff = (distdiff<0)?-distdiff:distdiff;
                    if( distdiff > DBL_MIN )
                    {
                    ++g_errorCount;
                    fprintf(stdout, "in testBigIntVector Left, apparently FarthestNeighbor has failed\n" );
                    }
                    }
                    });
    
    {
        /* somewhere in the middle, find a point and its nearest neighbor */
        /* make sure that each includes the other in sphere search */
        
        const intVec17 vBox17Center( RHrand::RHRAND_MAX/2 );
        intVec17 vNearCenter;
        intVec17 vCloseToNearCenter;
        const bool bFoundNear = tree.NearestNeighbor( (int)RHrand::RHRAND_MAX*17, vNearCenter, vBox17Center );
        
        if( ! bFoundNear )
        {
            ++g_errorCount;
            fprintf(stdout, "testBigIntVector: NearestNeighbor found no points\n" );
        }
        
        CNearTree<intVec17, int, -2> sphereReturn;
        // really, the only way to get the next section perfectly right is to do a 
        // binary search until exactly 2 points are found. (or do a linear search thru the points)
        const int radius = 2*(int)(RHrand::RHRAND_MAX/2*sqrt(17.)); // hand-tuned value
        unsigned long iFoundNearCenter = (unsigned long)tree.FindInSphere( radius, sphereReturn, vNearCenter );
        
        int searchRadius = radius/2;
        int delta        = searchRadius;
        int count = 0;
        while( iFoundNearCenter != 2 && count < 100 )
        {
            iFoundNearCenter = (unsigned long)tree.FindInSphere( searchRadius, sphereReturn, vNearCenter );
            if( iFoundNearCenter > 2 )
            {
                searchRadius = searchRadius - delta/2;
            }
            else if( iFoundNearCenter < 2 )
            {
                searchRadius = searchRadius + delta/2;
            }
            delta /= 2;
            ++count;
        }  // end while
        
        if( iFoundNearCenter < 2 )
        {
            ++g_errorCount;
            fprintf(stdout, "testBigIntVector: FindInSphere found %lu points, instead of 2\n", (unsigned long)iFoundNearCenter );
        }
        
        /* Brute force search for the point closest to the point closest to the center */
        double dmin = DBL_MAX;
        for( unsigned long i=0; i<iFoundNearCenter; ++i )
        {
            if( (vNearCenter - sphereReturn[i]).Norm( )!=0 && ( vNearCenter-sphereReturn[i] ).Norm( ) < dmin )
            {
                dmin = ( vNearCenter-sphereReturn[i] ).Norm( );
                vCloseToNearCenter = sphereReturn[i];
            }
        }
        
        {
            sphereReturn.clear( );
            const int radius2 = (int)( vCloseToNearCenter - vNearCenter ).Norm( );
            const unsigned long iSphereFoundNearCenter = (unsigned long)tree.FindInSphere( radius2, sphereReturn, vNearCenter );
            if( iSphereFoundNearCenter != 2 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FindInSphere failed to find only 2 points\n" );
            }
        }
        
        if( dmin == DBL_MAX ) 
        {
            ++g_errorCount;
            fprintf(stdout, "testBigIntVector: apparently FindInSphere failed\n" );
        }
        
        {
            /* Using zero radius, check that only one point is found when a point is searched
             with FindInSphere */
            sphereReturn.clear( );
            const long iFound = tree.FindInSphere( 0, sphereReturn, vNearCenter );
            if( iFound < 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FindInSphere found no points using zero radius\n" );
            }
            else if( iFound != 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FindInSphere found more than %ld points using zero radius\n", iFound );
            }
        }
        
        {
            /* Using minimal radius, check that at least 2 points are found when a point is searched
             with FindInSphere */
            sphereReturn.clear( );
            const long iFound = tree.FindInSphere( (vCloseToNearCenter-vNearCenter).Norm( ), sphereReturn, vNearCenter );
            if( iFound < 2 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FindInSphere found only 1 point\n" );
            }
        }
        
        {
            /* Using small radius, check that only one point is found when a point is searched
             with FindInSphere */
            sphereReturn.clear( );
            const long iFound = tree.FindInSphere( (int)((vCloseToNearCenter-vNearCenter).Norm( )*0.9), sphereReturn, vNearCenter );
            if( iFound < 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FindInSphere found no points using %f radius\n", (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
            }
            else if( iFound != 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FindInSphere found %ld points using %f radius\n", iFound, (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
            }
        }
        
        {
            const intVec17 vOrigin( 0 );
            const intVec17 vTopCorner( RHrand::RHRAND_MAX );
            tree.insert( vOrigin );
            tree.insert( vTopCorner );
            sphereReturn.clear( );
            intVec17 vReturn;
            const long iFound = tree.FarthestNeighbor( vReturn, vOrigin );
            if( iFound < 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FarthestNeighbor from origin found no points using %f radius\n", (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
            }
            else if( iFound != 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FarthestNeighbor from origin found %ld points using %f radius\n", iFound, (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
            }
            else if( (vReturn - vTopCorner ).Norm( ) != 0 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FarthestNeighbor from origin found wrong top point\n" );
            }
        }
        
        {
            const intVec17 vOrigin( 0 );
            const intVec17 vTopCorner( RHrand::RHRAND_MAX );
            tree.insert( vOrigin );
            tree.insert( vTopCorner );
            sphereReturn.clear( );
            intVec17 vReturn;
            const long iFound = tree.FarthestNeighbor( vReturn, vTopCorner );
            if( iFound < 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FarthestNeighbor from top found no points using %f radius\n", (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
            }
            else if( iFound != 1 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FarthestNeighbor from top found %ld points using %f radius\n", iFound, (vCloseToNearCenter-vNearCenter).Norm( )*0.9 );
            }
            else if( (vReturn - vOrigin ).Norm( ) != 0 )
            {
                ++g_errorCount;
                fprintf(stdout, "testBigIntVector: FarthestNeighbor from top found wrong top point\n" );
            }
        }
    }
} // testBigIntVec


/*=======================================================================*/
// test2Containers_InSphere
//
//  for FindInSphere : 
//  for testing both insertion of a container using a constructor and insertion
//  from a container and return of data using a container
/*=======================================================================*/
template< typename T1, typename T2 >
void test2Containers_InSphere( T1 & t1, T2 & t2 )
/*=======================================================================*/
{
    t1.clear( );
    t2.clear( );
    
    for( int i=0; i<10; ++i )
    {
        t1.insert( t1.end( ), i );
    }
    
    CNearTree<int> nt(t1);
    nt.insert( t1 );
    
    {
        const unsigned long nFound = nt.FindInSphere( 1.1, t2, 5 );
        
        if( nFound != 6 && nFound != 3 ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_InSphere: Found wrong # points %lu, expected 3 or 6\n", (unsigned long)nFound );
        }
        
        if( nFound != t2.size( ) && nFound/2 != t2.size( ) ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_InSphere: size(t2) %lu !=nFound\n", (unsigned long)t2.size( ) );
        }
    }
    {
        const unsigned long nFound = nt.LeftFindInSphere( 1.1, t2, 5 );
        
        if( nFound != 6 && nFound != 3 ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_LeftInSphere: Found wrong # points %lu, expected 3 or 6\n", (unsigned long)nFound );
        }
        
        if( nFound != t2.size( ) && nFound/2 != t2.size( ) ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_LeftInSphere: size(t2) %lu !=nFound\n", (unsigned long)t2.size( ) );
        }
    }
    
}  //  end test2Containers_InSphere

/*=======================================================================*/
// test2Containers_OutSphere
//
//  for FindOutSphere : 
//  for testing both insertion of a container using a constructor and insertion
//  from a container and return of data using a container
/*=======================================================================*/
template< typename T1, typename T2 >
void test2Containers_OutSphere( T1 & t1, T2 & t2 )
/*=======================================================================*/
{
    t1.clear( );
    t2.clear( );
    
    for( int i=0; i<10; ++i )
    {
        t1.insert( t1.end( ), i );
    }
    
    CNearTree<int> nt(t1);
    nt.insert( t1 );
    
    {
        const unsigned long nFound = nt.FindOutSphere( 1.1, t2, 5 );
        
        if( nFound != 14 && nFound != 7 ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_OutSphere: Found wrong # points %lu, expected 7 or 14\n", (unsigned long)nFound );
        }
        
        if( nFound != t2.size( ) && nFound/2 != t2.size( ) ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_OutSphere: size(t2) %lu !=nFound\n", (unsigned long)t2.size( ) );
        }
    };
    
    {
        const unsigned long nFound = nt.LeftFindOutSphere( 1.1, t2, 5 );
        
        if( nFound != 14 && nFound != 7 ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_LeftOutSphere: Found wrong # points %lu, expected 7 or 14\n", (unsigned long)nFound );
        }
        
        if( nFound != t2.size( ) && nFound/2 != t2.size( ) ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_LeftOutSphere: size(t2) %lu !=nFound\n", (unsigned long)t2.size( ) );
        }
    };
    
}  // end test2Containers_OutSphere

/*=======================================================================*/
// test2Containers_InAnnulus
//
//  for FindInAnnulus : 
//  for testing both insertion of a container using a constructor and insertion
//  from a container and return of data using a container
/*=======================================================================*/
template< typename T1, typename T2 >
void test2Containers_InAnnulus( T1 & t1, T2 & t2 )
/*=======================================================================*/
{
    t1.clear( );
    t2.clear( );
    
    for( int i=0; i<10; ++i )
    {
        t1.insert( t1.end( ), i );
    }
    
    CNearTree<int> nt(t1);
    nt.insert( t1 );
    {
        const unsigned long nFound = nt.FindInAnnulus( 1.1, 3.9, t2, 5 );
        
        if( nFound != 8 && nFound != 4 )
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_InAnnulus: Found wrong # points %lu, expected 8 or 4\n", (unsigned long)nFound );
        }
        
        if( nFound != t2.size( ) && nFound/2 != t2.size( ) ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_InAnnulus: size(t2) %lu !=nFound\n", (unsigned long)t2.size( ) );
        }
    };
    
    {
        const unsigned long nFound = nt.LeftFindInAnnulus( 1.1, 3.9, t2, 5 );
        
        if( nFound != 8 && nFound != 4 )
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_LeftInAnnulus: Found wrong # points %lu, expected 8 or 4\n", (unsigned long)nFound );
        }
        
        if( nFound != t2.size( ) && nFound/2 != t2.size( ) ) // sets do not allow duplicates
        {
            ++g_errorCount;
            fprintf(stdout, "test2Containers_LeftInAnnulus: size(t2) %lu !=nFound\n", (unsigned long)t2.size( ) );
        }
    };
    
}  //  end test2Containers_InAnnulus

/*=======================================================================*/
void testSTLContainerInput( void )
{
    
    {   std::vector<int> v;
        std::vector<int> vb;
        for( int i=0; i<10; ++i )
        {
            v.push_back( i );
        }
        
        {  // test the constructors
            CNearTree<int> tv(v);
            
            if( tv.size() != 10 )
            {
                ++g_errorCount;
                fprintf(stdout, "testSTLContainerInput: vector constructor has wrong count\n" );
            }
            
        }
        
        {  // test the inserters
            CNearTree<int> tv;
            
            tv.insert( v );
            
            if( tv.size() != 10 )
            {
                ++g_errorCount;
                fprintf(stdout, "testSTLContainerInput: vector insertion has wrong count\n" );
            }
            
        }
        
        {  // test the inserters
            CNearTree<int> tv;
            
            tv.insert( v );
            
            CNearTree<int> t;
            CNearTree<int> tb;
            test2Containers_InSphere( t, tb );
            test2Containers_InSphere( t, v );
            test2Containers_InSphere( v, tb );
            test2Containers_InSphere( v, vb );
            
            test2Containers_OutSphere( t, tb );
            test2Containers_OutSphere( t, v );
            test2Containers_OutSphere( v, tb );
            test2Containers_OutSphere( v, vb );
            
            test2Containers_InAnnulus( t, tb );
            test2Containers_InAnnulus( t, v );
            test2Containers_InAnnulus( v, tb );
            test2Containers_InAnnulus( v, vb );
            
        }
        
    }
    
    {   std::list<int> l;
        std::list<int> lb;
        for( int i=0; i<10; ++i )
        {
            l.push_back( i );
        }
        
        {  // test the constructors
            CNearTree<int> tl(l);
            
            if( tl.size() != 10 )
            {
                ++g_errorCount;
                fprintf(stdout, "testSTLContainerInput: list constructor has wrong count\n" );
            }
            
        }
        
        {  // test the inserters
            CNearTree<int> tl;
            
            tl.insert( l );
            
            if( tl.size() != 10 )
            {
                ++g_errorCount;
                fprintf(stdout, "testSTLContainerInput: list insertion has wrong count\n" );
            }
            
        }
        
        {  // test the inserters
            CNearTree<int> tl;
            
            tl.insert( l );
            
            CNearTree<int> t;
            CNearTree<int> tb;
            test2Containers_InSphere( t, tb );
            test2Containers_InSphere( t, l );
            test2Containers_InSphere( l, tb );
            test2Containers_InSphere( l, lb );
            
            test2Containers_OutSphere( t, tb );
            test2Containers_OutSphere( t, l );
            test2Containers_OutSphere( l, tb );
            test2Containers_OutSphere( l, lb );
            
            test2Containers_InAnnulus( t, tb );
            test2Containers_InAnnulus( t, l );
            test2Containers_InAnnulus( l, tb );
            test2Containers_InAnnulus( l, lb );
            
        }
        
    }
    
    { std::set<int> s;
        std::set<int> sb;
        for( int i=0; i<10; ++i )
        {
            s.insert( i );
        }
        
        {  // test the constructors
            CNearTree<int> ts(s);
            
            if( ts.size() != 10 )
            {
                ++g_errorCount;
                fprintf(stdout, "testSTLContainerInput: set constructor has wrong count\n" );
            }
            
        }
        
        {  // test the inserters
            CNearTree<int> ts;
            
            ts.insert( s );
            
            if( ts.size() != 10 )
            {
                ++g_errorCount;
                fprintf(stdout, "testSTLContainerInput: set insertion has wrong count\n" );
            }
            
        }
        
        {  // test the inserters
            CNearTree<int> ts;
            
            ts.insert( s );
            
            CNearTree<int> t;
            CNearTree<int> tb;
            test2Containers_InSphere( t, tb );
            test2Containers_InSphere( t, s );
            test2Containers_InSphere( s, tb );
            test2Containers_InSphere( s, sb );
            
            test2Containers_OutSphere( t, tb );
            test2Containers_OutSphere( t, s );
            test2Containers_OutSphere( s, tb );
            test2Containers_OutSphere( s, sb );
            
            test2Containers_InAnnulus( t, tb );
            test2Containers_InAnnulus( t, s );
            test2Containers_InAnnulus( s, tb );
            test2Containers_InAnnulus( s, sb );
            
        }
        
    }
} // testSTLContainerInput

/*=======================================================================*/
void testNearTreeInsertion ( void )
/*=======================================================================*/
{
    CNearTree<int> nt1;
    CNearTree<int> nt2;
    
    int count = 0;
    
    const int nInTree = 10;
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt1.insert( count++ );
    }
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt2.insert( count++ );
    }
    
    /* 
     insert a neartree
     */
    nt1.insert( nt2 );
    
    if( nt1.size() != 2*nInTree )
    {
        ++g_errorCount;
        fprintf( stdout, "testNearTreeInsertion: tree depth = %lu\n", (unsigned long)nt1.size( ) );
    }
}  // end testNearTreeInsertion

/*=======================================================================*/

void testTiming ( void )
{
    
    //const int nInsert = 1000000;
    
    //CNearTree<int> ntI;
    //for( int i=0; i<nInsert; ++i )
    //{
    //   ntI.insert( rand( ) );
    //}
    
    //const int nRetrieve = 1000000;
    //DWORD t0 = GetTickCount( );
    //for( int i=0; i<nRetrieve; ++i )
    //{
    //   ntI.NearestNeighbor( 1000, rand( ) );
    //}
    //const DWORD timediffI = GetTickCount () - t0;
    
    //CNearTree<double> ntD;
    //for( int i=0; i<nInsert; ++i )
    //{
    //   ntD.insert( (double)rand( ) );
    //}
    
    //t0 = GetTickCount( );
    //for( int i=0; i<nRetrieve; ++i )
    //{
    //   ntD.NearestNeighbor( 1000.0, (double)rand( ) );
    //}
    //const DWORD timediffD = GetTickCount( ) - t0;
    
    //fprintf( stdout, "timing for %d items in tree and %d retrievals, %ld, %ld\n", 
    //   nInsert, nRetrieve, timediffI, timediffD );
    
}  // end testTiming

/*=======================================================================*/
void testKNearFar( void )
{
    CNearTree<int> tree;
    CNearTree<int> outTree;
    std::vector<size_t> outIndices;
    
    {
        const int searchPoint = 50;
        const long nToFind0 = 0;
        const double radius0 = 1000.0;
        
        const size_t lFound0 = tree.FindK_NearestNeighbors( 
                                                           nToFind0,
                                                           radius0,
                                                           outTree,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound0 != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: #0: found wrong count %ld\n",  (long)lFound0);
        }
    }
    {
        const int searchPoint = 50;
        const long nToFind0 = 0;
        const double radius0 = 1000.0;
        
        const size_t lFound0 = tree.LeftFindK_NearestNeighbors( 
                                                           nToFind0,
                                                           radius0,
                                                           outTree,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound0 != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Left Near: #0: found wrong count %ld\n",  (long)lFound0);
        }
    }
    
    {
        const int searchPoint = 50;
        const long nToFind0 = 0;
        const double radius0 = 1000.0;
        
        const size_t lFound0 = tree.FindK_NearestNeighbors( 
                                                           nToFind0,
                                                           radius0,
                                                           outTree,
                                                           outIndices,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound0 != 0 || outIndices.size() != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: #0: found wrong count %ld and unexpected indices %ld \n",
                    (long)lFound0, (long)outIndices.size());
        }
    }
    
    {
        const int searchPoint = 50;
        const long nToFind0 = 0;
        const double radius0 = 1000.0;
        
       const size_t lFound0 = tree.LeftFindK_NearestNeighbors( 
                                                           nToFind0,
                                                           radius0,
                                                           outTree,
                                                           outIndices,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound0 != 0 || outIndices.size() != 0 )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Left Near: #0: found wrong count %ld and unexpected indices %ld \n",
                    (long)lFound0, (long)outIndices.size());
        }
    }
    
    
    for( int i=0; i<100; ++i )
    {
        tree.insert( i );
    }
    
    {
        const int searchPoint = 50;
        const long nToFind1 = 13;
        const double radius1 = 1000.0;
        
        const size_t lFound1 = tree.FindK_NearestNeighbors( 
                                                           nToFind1,
                                                           radius1,
                                                           outTree,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound1 != 13 )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: #1: found wrong count %ld\n", (long)lFound1 );
        }
    }

    {

        const int searchPoint = 50;
        const long nToFind1 = 13;
        const double radius1 = 1000.0;
        
        const size_t lFound1 = tree.LeftFindK_NearestNeighbors( 
                                                           nToFind1,
                                                           radius1,
                                                           outTree,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound1 != 13 )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Left Near: #1: found wrong count %ld\n", (long)lFound1 );
        }
    }
    
    {
        const int searchPoint = 50;
        const long nToFind1 = 13;
        const double radius1 = 1000.0;
        
        const size_t lFound1 = tree.FindK_NearestNeighbors( 
                                                           nToFind1,
                                                           radius1,
                                                           outTree,
                                                           outIndices,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound1 != 13 || outIndices.size() !=13)
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: #1: found wrong count %ld or wrong indices %ld\n",
                    (long)lFound1, (long)outIndices.size());
        } else {
            for (size_t ii = 0; ii < outIndices.size(); ii++) {
                if (tree[outIndices[ii]] != outTree[ii]) {
                    ++g_errorCount;
                    fprintf(stdout, "testKNearFar, Near: #1: mismatch between tree[%ld] and outTree[%ld]\n",
                            (long)outIndices[ii], (long)ii);
                }
            }
        }
    }

    {
        const int searchPoint = 50;
        const long nToFind1 = 13;
        const double radius1 = 1000.0;
        
        const size_t lFound1 = tree.LeftFindK_NearestNeighbors( 
                                                           nToFind1,
                                                           radius1,
                                                           outTree,
                                                           outIndices,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound1 != 13 || outIndices.size() !=13)
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Left Near: #1: found wrong count %ld or wrong indices %ld\n",
                    (long)lFound1, (long)outIndices.size());
        } else {
            for (size_t ii = 0; ii < outIndices.size(); ii++) {
                if (tree[outIndices[ii]] != outTree[ii]) {
                    ++g_errorCount;
                    fprintf(stdout, "testKNearFar, Left Near: #1: mismatch between tree[%ld] and outTree[%ld]\n",
                            (long)outIndices[ii], (long)ii);
                }
            }
        }
    }
    
    {
        const int searchPoint = 98;
        const long nToFind2 = 13;
        const double radius2 = 3.5;
        const size_t lFound2 = tree.FindK_NearestNeighbors( 
                                                           nToFind2,
                                                           radius2,
                                                           outTree,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound2 != 5 )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: found wrong count #2\n" );
        }
    }
    
    {
        const int searchPoint = 50;
        const long nToFind3 = 7;
        const double radius3 = 12;
        const size_t lFound3 = tree.FindK_NearestNeighbors( 
                                                           nToFind3,
                                                           radius3,
                                                           outTree,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound3 != 7u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: found wrong count #3\n" );
        }
    }
    
    {
        const int searchPoint = 2;
        const long nToFind4 = 7;
        const double radius4 = 12;
        const size_t lFound4 = tree.FindK_NearestNeighbors( 
                                                           nToFind4,
                                                           radius4,
                                                           outTree,
                                                           searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound4 != 7u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: found wrong count #4\n" );
        }
    }
    
    {
        const int searchPoint = 2;
        const long nToFind5 = 7;
        const double radius5 = 3.5;
        std::vector<int> outVector;
        const size_t lFound5 = tree.FindK_NearestNeighbors( 
                                                           nToFind5,
                                                           radius5,
                                                           outVector,
                                                           searchPoint );
        if( lFound5 != 6u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: found wrong count #5\n" );
        }
    }
    
    {
        const int searchPoint = 2;
        const long nToFind5 = 7;
        const double radius5 = 3.5;
        std::list<int> outList;
        const size_t lFound6 = tree.FindK_NearestNeighbors( 
                                                           nToFind5,
                                                           radius5,
                                                           outList,
                                                           searchPoint );
        if( lFound6 != 6u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: found wrong count #6\n" );
        }
    }

    {
        CNearTree<vec17> tree;
        CNearTree<vec17> outTree;
        const unsigned int n2Store = 20;

        for ( unsigned int i=0; i<n2Store; ++i )
        {
            tree.insert( vec17( ) );
        }

        const vec17 probe = vec17();
        const size_t n2Finda = 5;
        const size_t lFound17a = tree.FindK_NearestNeighbors( n2Finda, 100000.0, outTree, probe );
        if( lFound17a != n2Finda )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: found wrong count #17a testing lFound17a\n" );
        }
        if( lFound17a != outTree.size( ) )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: found wrong count #17a testing outTree.size()\n" );
        }
        double maxDist = -1.0;
        for ( unsigned int i=0; i<outTree.size(); ++i )
        {
            maxDist = std::max( maxDist, (probe-outTree[i]).Norm() );
        }

        outTree.clear( );
        const size_t lFound17b = tree.FindK_NearestNeighbors( 10*n2Store, 100000.0, outTree, probe );
        if( lFound17b != n2Store )
        {
            ++g_errorCount;
	    fprintf(stdout, "testKNearFar, Near: found wrong count #17b testing %ld, got %ld, expected %ld\n",
			                        (long int) outTree.size(), (long int) lFound17b, (long int) n2Store );
        }

        size_t count = 0;
        for ( unsigned int i=0; i<outTree.size(); ++i )
        {
            count += ((probe-outTree[i]).Norm() <= maxDist*(1.0+1.0E-9) ) ? 1 : 0 ;
        }
        if( count != n2Finda )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Near: found wrong count #17b testing %ld, got %ld, expected %ld\n",
                                                (long int) outTree.size(), (long int) n2Finda, (long int) count );
        }

    }
    
    {
        const int searchPoint = -1;
        const long nToFind1 = 13;
        
        const size_t lFound1 = tree.FindK_FarthestNeighbors( 
                                                            nToFind1,
                                                            outTree,
                                                            searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound1 != 13u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Far: found wrong count #1\n" );
        }
    }
    
    {
        const int searchPoint = 50;
        const long nToFind2 = 13;
        const size_t lFound2 = tree.FindK_FarthestNeighbors( 
                                                            nToFind2,
                                                            outTree,
                                                            searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound2 != 13 )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Far: found wrong count #2\n" );
        }
    }
    
    {
        const int searchPoint = 150;
        const long nToFind3 = 7;
        const size_t lFound3 = tree.FindK_FarthestNeighbors( 
                                                            nToFind3,
                                                            outTree,
                                                            searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound3 != 7u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Far: found wrong count #3\n" );
        }
    }
    
    {
        const int searchPoint = 46;
        const long nToFind4 = 12;
        const size_t lFound4 = tree.FindK_FarthestNeighbors( 
                                                            nToFind4,
                                                            outTree,
                                                            searchPoint );
        outTree.CompleteDelayedInsert( );
        if( lFound4 != 12u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Far: found wrong count #4\n" );
        }
    }
    
    {
        const int searchPoint = 2;
        const long nToFind5 = 7;
        std::vector<int> outVector;
        const size_t lFound5 = tree.FindK_FarthestNeighbors( 
                                                            nToFind5,
                                                            outVector,
                                                            searchPoint );
        if( lFound5 != 7u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Far: found wrong count #5\n" );
        }
    }
    
    {
        const int searchPoint = 200;
        const long nToFind6 = 7;
        std::list<int> outList;
        const size_t lFound6 = tree.FindK_FarthestNeighbors( 
                                                            nToFind6,
                                                            outList,
                                                            searchPoint );
        if( lFound6 != 7u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Far: found wrong count #6\n" );
        }
    }
    
    
    {
        const int searchPoint = 2;
        const long nToFind7 = 7;
        std::set<int> outSet;
        const size_t lFound7 = tree.FindK_FarthestNeighbors( 
                                                            nToFind7,
                                                            outSet,
                                                            searchPoint );
        if( lFound7 != 7u )
        {
            ++g_errorCount;
            fprintf(stdout, "testKNearFar, Far: found wrong count #7\n" );
        }
    }
    
}  // end testKNearFar

void testSeparation( void )
{
    CNearTree<int> t;
    const int imin = 0; 
    const int imax = 2000000;
    for ( int i=imin; i<imax; ++i )
    {
        t.insert( i );
    }

    typedef std::vector<int> container;
    container group1, group2;
    std::vector<size_t> group1_indices;
    std::vector<size_t> group2_indices;
    t.BelongsToPoints( imin, imax, group1, group2 );
    if ((long int)group1.size() != (long int)group2.size()) {
      ++g_errorCount;
      fprintf( stdout, "testSeparation: in %ld out %ld\n", (long int)group1.size(), (long int)group2.size() );
    }
    group1.clear();
    group2.clear();
    t.BelongsToPoints( imin, imax, group1, group2, group1_indices, group2_indices);
    if ((long int)group1.size() != (long int)group2.size()) {
        ++g_errorCount;
        fprintf( stdout, "testSeparation: in %ld out %ld\n", (long int)group1.size(), (long int)group2.size() );
    }
    if ((long int)group1_indices.size() != (long int)group1.size()) {
        ++g_errorCount;
        fprintf( stdout, "testSeparation: in indices %ld in %ld\n", (long int)group1_indices.size(), (long int)group2.size() );
    }
    if ((long int)group2_indices.size() != (long int)group2.size()) {
        ++g_errorCount;
        fprintf( stdout, "testSeparation: out indices %ld out %ld\n", (long int)group1_indices.size(), (long int)group2.size() );
    }
    for (size_t ii=0; ii <  group1.size(); ii++) {
        if (t[group1_indices[ii]] != group1[ii]) {
            ++g_errorCount;
            fprintf( stdout, "testSeparation: group1 index error index t[%ld] != group1[%ld] \n",
                    (long)group1_indices[ii], (long)ii);
        }
    }
    for (size_t ii=0; ii <  group2.size(); ii++) {
        if (t[group2_indices[ii]] != group2[ii]) {
            ++g_errorCount;
            fprintf( stdout, "testSeparation: group2 index error index t[%ld] != group2[%ld] \n",
                    (long)group2_indices[ii], (long)ii);
        }
    }
    
}

void testMergeConstructor( void )
{
    CNearTree<int> nt1;
    std::vector<int> nt2;
    
    int count = 0;
    
    const int nInTree = 10;
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt1.insert( count++ );
    }
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt2.push_back( count++ );
    }

    const CNearTree<int> test0( nt2 ); // one vector
    const CNearTree<int> test1( nt1, nt1 );// two neartrees
    const CNearTree<int> test2( nt1, nt2 );// neartree and a vector
    const CNearTree<int> test3( nt2, nt2 );// two vectors
    const long lFound = test2.size( );

    if( lFound != 2*nInTree )
    {
        ++g_errorCount;
        fprintf(stdout, "testMergeConstructor: found wrong count\n" );
    }
}

void testOperatorPlusEquals( void )
{
    CNearTree<int> nt1;
    std::vector<int> nt2;
    std::set<int> s2;
    
    int count = 0;
    
    const int nInTree = 10;
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt1.insert( count++ );
    }
    
    CNearTree<int> nt1a(nt1) ;
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt2.push_back( count );
        s2.insert( count );
        ++count;
    }

    nt1 += nt2;
    const long lFound = nt1.size( );

    if( lFound != 2*nInTree )
    {
        ++g_errorCount;
        fprintf(stdout, "testOperatorPlusEquals, found wrong count \n" );
    }

    
    nt1 = nt1a;
    nt1 += s2;
    const long lFounda = nt1.size( );

    if( lFounda != 2*nInTree )
    {
        ++g_errorCount;
        fprintf(stdout, "testOperatorPlusEquals-a, found wrong count \n" );
    }

    
}

void testOperatorMinusEquals( void )
{
    CNearTree<int> nt1;
    CNearTree<int> nt2;
    
    int count = 0;
    
    const int nInTree = 2*5; // tests below depend on this being an even number
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt1.insert( count++ );
    }
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt2.insert( 2*i );
    }

    nt1.CompleteDelayedInsert( );
    nt2.CompleteDelayedInsert( );

    nt1 -= nt2;
    const long lFound = nt1.size( );

    if( lFound != nInTree/2 )
    {
        ++g_errorCount;
        fprintf(stdout, "testOperatorMinusEquals, found wrong count \n" );
    }   
}

void testSetSymmetricDifference( void )
{
    CNearTree<int> nt1;
    CNearTree<int> nt2;
    std::vector<int> v2;
    
    const int nInTree = 2*5; // tests below depend on this being an even number
    
    for ( int i=0; i<nInTree; ++i )
    {

        nt1.insert( i );
    }
    
    CNearTree<int> nt1a = CNearTree<int>(nt1);
    
    for ( int i=0; i<nInTree; ++i )
    {
        nt2.insert( 2*i );
        v2.push_back( 2*i );
    }

    nt1.set_symmetric_difference( nt2 );
    const long lFound = nt1.size( );

    if( lFound != nInTree )
    {
        ++g_errorCount;
        fprintf(stdout, "testSetSymmetricDifference, found wrong count \n" );
    } 

    CNearTree<int> nt1b = CNearTree<int>(nt1a);
    nt1b.set_symmetric_difference( v2 );
    const long lFounda = nt1a.size( );

    if( lFounda != nInTree )
    {
        ++g_errorCount;
        fprintf(stdout, "testSetSymmetricDifference-a, found wrong count \n" );
    } 
}

void testCentroid( )
{
    {
        std::vector<int> v;
        for ( int i=0; i<20; ++i )
        {
            v.push_back( i );
        }

        CNearTree<int> nt( v );

        const double cm1 = nt.Centroid( ); 
        const double cm2 = CNearTree<int>::Centroid( v ); 

        if ( cm1 != cm2 )
        {
            ++g_errorCount;
            fprintf(stdout, "testCentroid-a, cm1 %f  cm2 %f\n", cm1, cm2 );
        }
    }

    {
        std::vector<double> v;
        for ( int i=0; i<20; ++i )
        {
            v.push_back( i );
        }

        CNearTree<double> nt( v );

        const double cm1 = nt.Centroid( ); 
        const double cm2 = CNearTree<double>::Centroid( v ); 

        if ( cm1 != cm2 )
        {
            ++g_errorCount;
            fprintf(stdout, "testCentroid-b, cm1 %f  cm2 %f\n", cm1, cm2 );
        }
    }
}

template< typename T >
std::vector<T> LloydCycleStep( const CNearTree<T>& coord, const std::vector<T>& vIn )
{
    std::vector<T> v(vIn);
    std::vector<T> vSum(v.size( ), 0); 
    std::vector<T> vCount( v.size( ) );

    typename CNearTree<T>::iterator it;
    for ( it=coord.begin( ); it!=coord.end( ); ++it )
    {
        double dmin = DBL_MAX;
        int indexVSumMin=0;
        int indexVMin = 0;
        for ( unsigned int i=0; i<v.size( ); ++i )
        {
            if ( abs((*it)-v[i]) < dmin )
            {
                dmin = abs((*it)-v[i]);
                indexVMin = it.get_position( );
                indexVSumMin = i;
            }
        }

        vSum[indexVSumMin] += coord[indexVMin];
        ++(vCount[indexVSumMin]);
    }

    for ( unsigned int i=0; i<vSum.size( ); ++i )
    {
        if ( vCount[i] > 0.0 )
        {
           vSum[i] /= double(vCount[i]);
        }
    }

    return( vSum );
}

void testLloyd( )
{
    CNearTree<double> vdata;
    std::vector<double> vk;

    for ( int i=0; i<20000; ++i )
    {
        vdata.insert( double(i) );
    }    
    
    size_t estdim = (size_t)(0.5+vdata.GetDimEstimate());
    if ( estdim != 1) {
        ++g_errorCount;   
    fprintf(stdout, "testLloyd: dimension estimate %ld != 1\n",(long)estdim);
    }
    
    vk.push_back( double(-12) );
    vk.push_back( double(0) );
    vk.push_back( double(17) );

    for ( int i=0; i<60; ++i  )
    {
        std::vector<double> vOut = LloydCycleStep( vdata, vk );
        if (i > 35 && (vk[0] != 3333. || vk[1] != 10000. || vk[2] != 16666.5)) {
            ++g_errorCount;
            fprintf( stdout, "testLoyd cycle %d ", i);
            for ( unsigned int j=0; j<vk.size( ); ++j )
            {
                fprintf( stdout, "%.3f ", vk[j] );
            }
            fprintf( stdout, "\n" );            
        } else {
            if (i > 35) break;
        }
        vk = vOut;
    }
}

//> Lloyd cycle step
//>
//> put starting points (SPs) in a neartree
//>
//> CNearTree LloydCycle( const CNearTree& data, const CNearTree& SP )
//> make a vector v of Vector_3 of size SP.size( ) and one of long same size
//> for each data point
//>   get iterator to nearest SP
//>   v[it->GetPosition()] += *it;
//>   ++count[it->GetPosition()];
//> end for
//>
//> foreach in v
//>   v[i] /= count[i]
//>
//> return (CNearTree(v));

