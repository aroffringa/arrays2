//# tArrayIO3.cc: This program tests the ArrayIO istream extract  functions
//# Copyright (C) 1993,1994,1995,1996,1997,1999,2000,2002,2003
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
#include "../ArrayIO.h"
#include "../Array.h"
#include "../Vector.h"
#include "../Matrix.h"
#include "../Cube.h"

#include <boost/test/unit_test.hpp>

#include <casacore/casa/BasicSL/String.h>
#include <casacore/casa/Quanta/Quantum.h>

using namespace casacore::array2;

// This test program tests the ArrayIO istream extract functions.

BOOST_AUTO_TEST_SUITE(array_IO_3)

template<typename Arr>
void checkExtract(const std::string& str, const Arr& ref, const std::string& outputStr)
{
  std::stringstream istr(str);
  Arr a;
  istr >> a;
  BOOST_CHECK(istr);
  BOOST_CHECK_EQUAL(a.shape().size(), ref.shape().size());
  BOOST_CHECK_EQUAL(a.shape(), ref.shape());
  BOOST_CHECK_EQUAL(a.shape(), ref.shape());
  std::ostringstream astr, refstr;
  astr << a;
  refstr << ref;
  BOOST_CHECK_EQUAL(astr.str(), refstr.str());
  BOOST_CHECK_EQUAL(astr.str(), outputStr);
}

BOOST_AUTO_TEST_CASE( double_istream_extract_arr1 )
{
  checkExtract(
    "[1 2 3 4 5 6]",
    Array<double>{1,2,3,4,5,6},
    "[1, 2, 3, 4, 5, 6]"
  );
}

BOOST_AUTO_TEST_CASE( double_istream_extract_matrix1 )
{
  const double data[] = {1,2,3,4,5,6};
  checkExtract(
    "[1 2 3 4 5 6]\n",
    Matrix<double>(IPosition{6, 1}, data),
    "Axis Lengths: [6, 1]  (NB: Matrix in Row/Column order)\n"
      "[1\n"
      " 2\n"
      " 3\n"
      " 4\n"
      " 5\n"
      " 6]\n"
  );
}

BOOST_AUTO_TEST_CASE( double_istream_extract_arr2 )
{
  const double data[] = {1,4,2,5,3,6};
  checkExtract(
    "{[2 3]}[1 2 3 4 5 6]\n",
    Array<double>(IPosition{2, 3}, data),
    "Axis Lengths: [2, 3]  (NB: Matrix in Row/Column order)\n"
      "[1, 2, 3\n"
      " 4, 5, 6]\n"
  );
}

BOOST_AUTO_TEST_CASE( double_istream_extract_matrix2 )
{
  const double data[] = {1,4,2,5,3,6};
  checkExtract(
    "{[2 3]}[1 2 3 4 5 6]\n",
    Matrix<double>(IPosition{2, 3}, data),
    "Axis Lengths: [2, 3]  (NB: Matrix in Row/Column order)\n"
      "[1, 2, 3\n"
      " 4, 5, 6]\n"
  );
}

BOOST_AUTO_TEST_CASE( double_istream_extract_arr3 )
{
  const double data[] = {1,2,3,4,5,6};
  checkExtract(
    "{t[2 3]}[1 2 3 4 5 6]\n",
    Array<double>(IPosition{2, 3}, data),
    "Axis Lengths: [2, 3]  (NB: Matrix in Row/Column order)\n"
      "[1, 3, 5\n"
      " 2, 4, 6]\n"
  );
}

BOOST_AUTO_TEST_CASE( double_istream_extract_quantity )
{
  casacore::Quantity a(1, "s"), b(5.4, "Jy"), c(92, "pc/a");
  checkExtract(
    "[1s 5.4Jy 92pc/a ]\n",
    Array<casacore::Quantity>{a,b,c},
    "[1 s, 5.4 Jy, 92 pc/a]"
  );
}

BOOST_AUTO_TEST_CASE( double_istream_extract_string1 )
{
  checkExtract(
    "[ x, y , x]\n",
    Array<std::string>{"x","y","x"},
    "[x, y, x]"
  );
}

BOOST_AUTO_TEST_CASE( double_istream_extract_string2 )
{
  checkExtract(
    "[x, y]\n",
    Array<std::string>{"x","y"},
    "[x, y]"
  );
}

BOOST_AUTO_TEST_SUITE_END()
