//*
//*  v.h
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

// v.h: interface for the v class.
//
// This is just a trivial vector class in 3-space for the purpose
// of demonstrating the nearest neighbor template class. It is not
// designed to do anything important, and, as a consequence, it is
// very incomplete.
//
//  larry andrews, 2001
//
//////////////////////////////////////////////////////////////////////

#if !defined(V_H_INCLUDED)
#define V_H_INCLUDED


// #include <ostream.h>

class v  
{
public:
   v  operator- ( const v& v ) const;
   bool operator< ( const v& v ) const;

   operator double( ) const;
   v  ( const double& da, const double& db, const double& dc );
   v  ( const double& da );
   v  ( );
   ~v ( );
   double Norm( void ) const;

friend std::ostream& operator <<( std::ostream& os, const v& v );

private:
   bool   m_bIsDefined;
   double m_dvec[ 3 ];
};

#endif // !defined(V_H_INCLUDED)
