//*
//*  v.cpp
//*  NearTree
//*
//*  Copyright 2001, 2008 Larry Andrews.  All rights reserved
//*  Revised 12 Dec 2008 for sourceforge release -- H. J. Bernstein


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

// v.cpp: implementation of the v class.
//
//////////////////////////////////////////////////////////////////////
#include <float.h>
#include <math.h>
#include <ostream>
#include "v.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

v::v( )
{
	m_bIsDefined	= false;
	m_dvec[0]		= DBL_MAX;
	m_dvec[1]		= DBL_MAX;
	m_dvec[2]		= DBL_MAX;
}

v::~v( )
{
	m_bIsDefined	= false;
	m_dvec[0]		= DBL_MAX;
	m_dvec[1]		= DBL_MAX;
	m_dvec[2]		= DBL_MAX;
}

v::v( const double& da )
{
	m_bIsDefined	= true;
	m_dvec[0]		= da;
	m_dvec[1]		= da;
	m_dvec[2]		= da;

}

v::v( const double& da, const double& db, const double& dc )
{
	m_bIsDefined	= true;
	m_dvec[0]		= da;
	m_dvec[1]		= db;
	m_dvec[2]		= dc;
}


//////////////////////////////////////////////////////////////////////
// Other Stuff
//////////////////////////////////////////////////////////////////////
std::ostream& operator<< ( std::ostream& os, const v& v )
{
   // output a vector
   os << '{' <<v.m_dvec[0] << ',' <<  v.m_dvec[1]  << ',' <<  v.m_dvec[2] <<'}' ;
   return os;
}

v v::operator -(const v &vv) const
{
   // subtract two vectors
   v vReturn(
		m_dvec[0]-vv.m_dvec[0],
		m_dvec[1]-vv.m_dvec[1],
		m_dvec[2]-vv.m_dvec[2] );
   return ( vReturn );
}

bool v::operator <(const v &vv) const
{
    // subtract two vectors
    bool vReturn(
              m_dvec[0]<vv.m_dvec[0] ||
              (m_dvec[0]==vv.m_dvec[0] && m_dvec[1]<vv.m_dvec[1])  ||
              (m_dvec[0]==vv.m_dvec[0] && m_dvec[1]== vv.m_dvec[1] && m_dvec[2]<vv.m_dvec[2] ));
    return ( vReturn );
}

v::operator double( ) const
{
   // return the Euclidean (L2) length of the vector
	return ( sqrt( m_dvec[0]*m_dvec[0] + m_dvec[1]*m_dvec[1] + m_dvec[2]*m_dvec[2] ) );
   // city block measure (L1) 	return ( fabs(m_dvec[0]) + fabs(m_dvec[1]) + fabs(m_dvec[2]) );
   // max value (L-infinity) return the largest of fabs of any element
   // return ( fabs(m_dvec[0])>=fabs(m_dvec[1]) && fabs(m_dvec[0])>=fabs(m_dvec[2]) ? fabs(m_dvec[0]) : fabs(m_dvec[1])>=fabs(m_dvec[2]) ? fabs(m_dvec[1]) : fabs(m_dvec[2]) );
   // Hamming measure (if this is a difference vector) return ( (m_dvec[0]==0 ? 0 : 1) + (m_dvec[1]==0 ? 0 : 1) + (m_dvec[2]==0 ? 0 : 1) )
}

double v::Norm( void ) const
{
	return( sqrt( m_dvec[0]*m_dvec[0] + m_dvec[1]*m_dvec[1] + m_dvec[2]*m_dvec[2] ) );

}
