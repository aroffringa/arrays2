//# tMaskArrLogi.cc: Test program for MaskedArray logical operations
//# Copyright (C) 1993,1994,1995,1996,1999,2000,2001
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

//# If AIPS_DEBUG is not set, the Assert's won't be called.
#if !defined(AIPS_DEBUG)
#define AIPS_DEBUG
#endif

//# For extra debugging
#if !defined(AIPS_ARRAY_INDEX_CHECK)
#define AIPS_ARRAY_INDEX_CHECK
#endif

#include <casacore/casa/iostream.h>

#include <casacore/casa/aips.h>
#include <casacore/casa/Utilities/Assert.h>
#include <casacore/casa/BasicSL/Complex.h>
#include <casacore/casa/BasicMath/Math.h>
#include <casacore/casa/BasicSL/String.h>

#include <casacore/casa/Arrays/IPosition.h>
#include <casacore/casa/Arrays/Array.h>
#include <casacore/casa/Arrays/ArrayError.h>
#include <casacore/casa/Arrays/ArrayIO.h>
#include <casacore/casa/Arrays/ArrayLogical.h>
#include <casacore/casa/Arrays/ArrayMath.h>
#include <casacore/casa/Arrays/Vector.h>
#include <casacore/casa/Arrays/Matrix.h>
#include <casacore/casa/Arrays/Cube.h>
#include <casacore/casa/Arrays/LogiVector.h>

#include <casacore/casa/Arrays/MaskedArray.h>
#include <casacore/casa/Arrays/MaskArrIO.h>
#include <casacore/casa/Arrays/MaskArrLogi.h>
#include <casacore/casa/Arrays/MaskArrMath.h>


#include <casacore/casa/namespace.h>

int main()
{
    try {
        cout << endl << "Testing MaskedArray logical operators." << endl;

        Vector<int> u(10), v(10), w(10), x(10), y(10), z(10);
        LogicalArray b(IPosition(1,10));

        indgen (u, -2);
        u(0) = 8;
        u(1) = 9;
        cout << endl << "u= " << endl
             << u << endl;

        v=-1;
        cout << endl << "v= " << endl
             << v << endl;

        w=11;
        cout << endl << "w= " << endl
             << w << endl;

        x=1;
        cout << endl << "x= " << endl
             << x << endl;

        indgen (y);
        cout << endl << "y= " << endl
             << y << endl;

        z=5;
        cout << endl << "z= " << endl
             << z << endl;

        b = ((y > 3) && (y < 8));
        cout << endl << "b= " << endl
             << b << endl;

        {
            cout << endl << "Test MaskedArray::MaskedArray (MaskedArray," 
                            " LogicalArray)" << endl;

            Vector<int> a(x.copy());
            MaskedArray<int> ma (a, b);
            MaskedArray<int> mma (ma, y>5);

            mma = 75;
            cout << "mma = 75;" << endl
                 << a << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator () (LogicalArray)"
                 << endl;

            Vector<int> a(x.copy());
            MaskedArray<int> ma (a, b);

            ma (y>5) = 75;
            cout << "ma (y>5) = 75;" << endl
                 << a << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator <= (MaskedArray, Array)"
                 << endl;

            cout << "(y(b) <= z).getArray()" << endl
                 << (y(b) <= z).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator <= (Array, MaskedArray)"
                 << endl;

            cout << "(y <= z(b)).getArray()" << endl
                 << (y <= z(b)).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator <= (MaskedArray,"
                            " MaskedArray)"
                 << endl;

            cout << "(y(b) <= z(y>4)).getArray()" << endl
                 << (y(b) <= z(y>4)).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator <= (MaskedArray,"
                            " Scalar)"
                 << endl;

            cout << "(y(b) <= 5).getArray()" << endl
                 << (y(b) <= 5).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator <= (Scalar,"
                            " MaskedArray)"
                 << endl;

            cout << "(5 > y(b)).getArray()" << endl
                 << (5 > y(b)).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator && (MaskedArray,"
                            " Scalar)"
                 << endl;

            cout << "((5 > y(b)) && true).getArray()" << endl
                 << ((5 > y(b)) && true).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator && (MaskedArray,"
                            " Scalar)"
                 << endl;

            cout << "((5 > y(b)) && false).getArray()" << endl
                 << ((5 > y(b)) && false).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator || (MaskedArray,"
                            " Scalar)"
                 << endl;

            cout << "((5 > y(b)) || true).getArray()" << endl
                 << ((5 > y(b)) || true).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator || (MaskedArray,"
                            " Scalar)"
                 << endl;

            cout << "((5 > y(b)) || false).getArray()" << endl
                 << ((5 > y(b)) || false).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator && (Scalar,"
                            " MaskedArray)"
                 << endl;

            cout << "(true && (5 > y(b))).getArray()" << endl
                 << (true && (5 > y(b))).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator && (Scalar,"
                            " MaskedArray)"
                 << endl;

            cout << "(false && (5 > y(b))).getArray()" << endl
                 << (false && (5 > y(b))).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator || (Scalar,"
                            " MaskedArray)"
                 << endl;

            cout << "(true || (5 > y(b))).getArray()" << endl
                 << (true || (5 > y(b))).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator || (Scalar,"
                            " MaskedArray)"
                 << endl;

            cout << "(false || (5 > y(b))).getArray()" << endl
                 << (false || (5 > y(b))).getArray() << endl;
        }

        {
            cout << endl << "Test MaskedArray::operator ! (MaskedArray)"
                 << endl;

            Vector<int> a_log (10);
            indgen (a_log);

            LogicalVector b_log ((a_log > 1) && (a_log < 6));

            cout << endl << "a_log= " << endl
                 << a_log << endl;
            cout << "b_log= " << endl
                 << b_log << endl;

            MaskedLogicalArray mla_log ((a_log >= 3), b_log);
            cout << "mla_log= " << endl
                 << mla_log << endl;

            cout << endl << "! mla_log = " << endl;
            cout <<          ! mla_log << endl;
            cout << "(a_log < 3)(b_log) = " << endl;
            cout <<  (a_log < 3)(b_log) << endl;
            cout << "allEQ (!mla_log, (a_log < 3)(b_log)) = "
                 << allEQ (!mla_log, (a_log < 3)(b_log)) << endl;

        }

        {
            cout << endl << "Test MaskedArray::allLE (MaskedArray, Array)"
                 << endl;

            cout << "allLE (y(b), z)= "
                 << allLE (y(b), z) << endl;
            cout << "allLE (y(b), w)= "
                 << allLE (y(b), w) << endl;
        }

        {
            cout << endl << "Test MaskedArray::allGT (Array, MaskedArray)"
                 << endl;

            cout << "allGT (z, y(b))= "
                 << allGT (z, y(b)) << endl;
            cout << "allGT (w, y(b))= "
                 << allGT (w, y(b)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::allLT (MaskedArray, MaskedArray)"
                 << endl;

            cout << "allLT (u(u<9), y(y<7))= "
                 << allLT (u(u<9), y(y<7)) << endl;
            cout << "allLT (u(u<8), y(y<7))= "
                 << allLT (u(u<8), y(y<7)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyLE (MaskedArray, Array)"
                 << endl;

            cout << "anyLE (y(b), z)= "
                 << anyLE (y(b), z) << endl;
            cout << "anyLE (y(b), v)= "
                 << anyLE (y(b), v) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyGT (Array, MaskedArray)"
                 << endl;

            cout << "anyGT (z, y(b))= "
                 << anyGT (z, y(b)) << endl;
            cout << "anyGT (v, y(b))= "
                 << anyGT (v, y(b)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyLT (MaskedArray, MaskedArray)"
                 << endl;

            cout << "anyLT (u(u<9), y(y<7))= "
                 << anyLT (u(u<9), y(y<7)) << endl;
            cout << "anyLT (u(u>8), y(y<7))= "
                 << anyLT (u(u>8), y(y<7)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyAND (MaskedArray, Array)"
                 << endl;

            cout << "anyAND (b(y>5), y>4)= "
                 << anyAND (b(y>5), y>4) << endl;
            cout << "anyAND (b(y==4), y>4)= "
                 << anyAND (b(y==4), y>4) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyAND (Array, MaskedArray)"
                 << endl;

            cout << "anyAND (y>4, b(y>5))= "
                 << anyAND (y>4, b(y>5)) << endl;
            cout << "anyAND (y>4, b(y==4))= "
                 << anyAND (y>4, b(y==4)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyAND (MaskedArray, MaskedArray)"
                 << endl;

            cout << "anyAND (b(y>4), b(y>5))= "
                 << anyAND (b(y>4), b(y>5)) << endl;
            cout << "anyAND (b(y<3), b(y<=3))= "
                 << anyAND (b(y<3), b(y<=3)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyOR (MaskedArray, Array)"
                 << endl;

            cout << "anyOR (b(y>5), y>4)= "
                 << anyOR (b(y>5), y>4) << endl;
            cout << "anyOR (b(y==3), y>4)= "
                 << anyOR (b(y==3), y>4) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyOR (Array, MaskedArray)"
                 << endl;

            cout << "anyOR (y>4, b(y>5))= "
                 << anyOR (y>4, b(y>5)) << endl;
            cout << "anyOR (y>4, b(y==3))= "
                 << anyOR (y>4, b(y==3)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyOR (MaskedArray, MaskedArray)"
                 << endl;

            cout << "anyOR (b(y>4), b(y>5))= "
                 << anyOR (b(y>4), b(y>5)) << endl;
            cout << "anyOR (b(y<3), b(y<=3))= "
                 << anyOR (b(y<3), b(y<=3)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::allLT (MaskedArray, Scalar)"
                 << endl;

            cout << "allLT (y(y>5), 7)= "
                 << allLT (y(y>5), 7) << endl;
            cout << "allLT (y(y>5), 11)= "
                 << allLT (y(y>5), 11) << endl;
        }

        {
            cout << endl << "Test MaskedArray::allGE (Scalar, MaskedArray)"
                 << endl;

            cout << "allGE (7, y(y>5))= "
                 << allGE (7, y(y>5)) << endl;
            cout << "allGE (11, y(y>5))= "
                 << allGE (11, y(y>5)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyLT (MaskedArray, Scalar)"
                 << endl;

            cout << "anyLT (y(y>5), 7)= "
                 << anyLT (y(y>5), 7) << endl;
            cout << "anyLT (y(y>5), 5)= "
                 << anyLT (y(y>5), 5) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyGE (Scalar, MaskedArray)"
                 << endl;

            cout << "anyGE (7, y(y>5))= "
                 << anyGE (7, y(y>5)) << endl;
            cout << "anyGE (5, y(y>5))= "
                 << anyGE (5, y(y>5)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::allAND (MaskedArray, bool)"
                 << endl;

            cout << "allAND (b(y>5), true)= "
                 << allAND (b(y>5), true) << endl;
            cout << "allAND (b(b), true)= "
                 << allAND (b(b), true) << endl;
            cout << "allAND (b(b), false)= "
                 << allAND (b(b), false) << endl;
        }

        {
            cout << endl << "Test MaskedArray::allAND (bool, MaskedArray)"
                 << endl;

            cout << "allAND (true, b(y>5))= "
                 << allAND (true, b(y>5)) << endl;
            cout << "allAND (true, b(b))= "
                 << allAND (true, b(b)) << endl;
            cout << "allAND (false, b(b))= "
                 << allAND (false, b(b)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::allOR (MaskedArray, bool)"
                 << endl;

            cout << "allOR (b(y>5), false)= "
                 << allOR (b(y>5), false) << endl;
            cout << "allOR (b(b), false)= "
                 << allOR (b(b), false) << endl;
            cout << "allOR (b(y>5), true)= "
                 << allOR (b(y>5), true) << endl;
        }

        {
            cout << endl << "Test MaskedArray::allOR (bool, MaskedArray)"
                 << endl;

            cout << "allOR (false, b(y>5))= "
                 << allOR (false, b(y>5)) << endl;
            cout << "allOR (false, b(b))= "
                 << allOR (false, b(b)) << endl;
            cout << "allOR (true, b(y>5))= "
                 << allOR (true, b(y>5)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyAND (MaskedArray, bool)"
                 << endl;

            cout << "anyAND (b(y>5), true)= "
                 << anyAND (b(y>5), true) << endl;
            cout << "anyAND (b(y<3), true)= "
                 << anyAND (b(y<3), true) << endl;
            cout << "anyAND (b(y>5), false)= "
                 << anyAND (b(y>5), false) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyAND (bool, MaskedArray)"
                 << endl;

            cout << "anyAND (true, b(y>5))= "
                 << anyAND (true, b(y>5)) << endl;
            cout << "anyAND (true, b(y<3))= "
                 << anyAND (true, b(y<3)) << endl;
            cout << "anyAND (false, b(y>5))= "
                 << anyAND (false, b(y>5)) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyOR (MaskedArray, bool)"
                 << endl;

            cout << "anyOR (b(y>5), false)= "
                 << anyOR (b(y>5), false) << endl;
            cout << "anyOR (b(y<3), false)= "
                 << anyOR (b(y<3), false) << endl;
            cout << "anyOR (b(y<3), true)= "
                 << anyOR (b(y<3), true) << endl;
        }

        {
            cout << endl << "Test MaskedArray::anyOR (bool, MaskedArray)"
                 << endl;

            cout << "anyOR (false, b(y>5))= "
                 << anyOR (false, b(y>5)) << endl;
            cout << "anyOR (false, b(y<3))= "
                 << anyOR (false, b(y<3)) << endl;
            cout << "anyOR (true, b(y<3))= "
                 << anyOR (true, b(y<3)) << endl;
        }

    } catch (AipsError& x) {
        cout << "\nCaught an exception: " << x.getMesg() << endl;
    } 

    cout << endl << "OK" << endl;
    return 0;
}
