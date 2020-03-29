//# tArrayIO.cc: This program tests Array IO
//# Copyright (C) 1993,1994,1995,1996,1999,2002
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This program is free software; you can redistribute it and/or modify it
//# under the terms of the GNU General Public License as published by the Free
//# Software Foundation; either version 2 of the License, or (at your option)
//# any later version.
//#
//# This program is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//# more details.
//#
//# You should have received a copy of the GNU General Public License along
//# with this program; if not, write to the Free Software Foundation, Inc.,
//# 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id$

//# Includes

#include "../Array.h"
#include "../ArrayError.h"
#include "../ArrayLogical.h"
#include "../ArrayIO.h"
#include "../ArrayMath.h"
#include "../IPosition.h"
#include "../Vector.h"
#include "../Matrix.h"
#include "../Cube.h"

#include <boost/test/unit_test.hpp>

using namespace casacore::array2;

BOOST_AUTO_TEST_SUITE(array_IO_1)

BOOST_AUTO_TEST_CASE( stream )
{
  // Write -
  casacore::AipsIO io("arrtest.out", casacore::ByteIO::New);

  Vector<int> v(100);
  indgen(v);
  io << v;

  Matrix<double> y(10,10);
  y = 1.0;
  y.diagonal() = 5.0;
  io << y;

  Cube<int> z(4,4,4);
  z = 4;
  io << z;

  IPosition shape(4);
  shape = 3;
  Array<int> a(shape);
  a = 33;
  io << a;

  io.close();
  io.open("arrtest.out", casacore::ByteIO::Old);

  Vector<int> v2;
  Matrix<double> y2;
  Cube<int> z2;
  Array<int> a2;
  io >> v2 >> y2 >> z2 >> a2;
    
  BOOST_CHECK_EQUAL(v.shape(), v2.shape());
  BOOST_CHECK(allEQ (v, v2));
  BOOST_CHECK_EQUAL(y.shape(), y2.shape());
  BOOST_CHECK(allEQ (y, y2));
  BOOST_CHECK_EQUAL(z.shape(), z2.shape());
  BOOST_CHECK(allEQ (z, z2));
  BOOST_CHECK_EQUAL(a.shape(), a2.shape());
  BOOST_CHECK(allEQ (a, a2));

  io.close();
  io.open("arrtest.out", casacore::ByteIO::Delete);
  io.close();
}

BOOST_AUTO_TEST_SUITE_END()
