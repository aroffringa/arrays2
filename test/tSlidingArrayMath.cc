//# tSlidingArrayMath.cc: Test program for function slidingArrayMath
//# Copyright (C) 2006
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id$

#include <casacore/casa/Arrays/MaskArrMath.h>
#include <casacore/casa/Arrays/ArrayLogical.h>
#include <casacore/casa/Arrays/ArrayPartMath.h>
#include <casacore/casa/Arrays/ArrayIO.h>

#include <casacore/casa/Utilities/GenSort.h>
#include <casacore/casa/OS/Timer.h>
#include <iostream>


#ifdef AIPS_NO_TEMPLATE_SRC
#include <casacore/casa/Arrays/ArrayMath.tcc>
#include <casacore/casa/Arrays/MaskArrMath.tcc>
namespace casacore {
  template Array<float> slidingArrayMath (const Array<float>&,
					  const IPosition&,
					  float (*) (const Array<float>&),
					  bool);
  template Array<float> slidingArrayMath (const MaskedArray<float>&,
					  const IPosition&,
					  float (*) (const MaskedArray<float>&),
					  bool);
}
#endif //# AIPS_NO_TEMPLATE_SRC


using namespace casacore;
using namespace std;

float smartMedian (const Array<float>& arr)
{
  Block<float> bbuf(arr.size());
  float* buf = bbuf.storage();
  int nl = 0;
  int nr = arr.size();
  float pivot = arr(arr.shape()/2);
  Array<float>::const_iterator iterEnd=arr.end();
  for (Array<float>::const_iterator iter=arr.begin();
       iter!=iterEnd;
       ++iter) {
    float val = *iter;
    if (val <= pivot) {
      buf[nl++] = val;
    } else {
      buf[--nr] = val;
    }
  }
  if (nl >= nr/2) {
    return GenSort<float>::kthLargest (buf, nl, nr/2);
  } else {
    return GenSort<float>::kthLargest (buf+nl, nr-nl, nr/2-nl);
  }
}

void doIt (bool doTiming)
{
  {
    IPosition shape(2,5,5);
    Array<float> arr(shape);
    indgen (arr);
    cout << slidingArrayMath(arr, IPosition(2,2), SumFunc<float>(), false) << endl;
    cout << slidingArrayMath(arr, IPosition(2,1), SumFunc<float>(), false) << endl;
    cout << slidingArrayMath(arr, IPosition(1,0), SumFunc<float>(), false) << endl;

    cout << slidingArrayMath(arr, IPosition(4,2), SumFunc<float>(), true) << endl;
    cout << slidingArrayMath(arr, IPosition(3,1), SumFunc<float>(), true) << endl;
    cout << slidingArrayMath(arr, IPosition(),    SumFunc<float>(), true) << endl;

    cout << slidingArrayMath(arr, IPosition(2,2), MedianFunc<float>(), false) << endl;
    cout << slidingArrayMath(arr, IPosition(2,1), MedianFunc<float>(), false) << endl;
    cout << slidingArrayMath(arr, IPosition(2,0), MedianFunc<float>(), false) << endl;
  }

  if (doTiming) {
    Array<float> arr(IPosition(2,1000,1000));
    indgen(arr);
    Timer timer;
    Array<float> res = slidingArrayMath(arr, IPosition(2,25), MedianFunc<float>());
    timer.show();
    timer.mark();
    {
      IPosition blc(2,0);
      IPosition trc(2,30);
      for (int i=0; i<100000; i++) {
	median(arr(blc,trc));
      }
    }
    timer.show();
    timer.mark();
    {
      IPosition blc(2,0);
      IPosition trc(2,30);
      for (int i=0; i<100000; i++) {
	smartMedian(arr(blc,trc));
      }
    }
    timer.show();
    timer.mark();
    {
      IPosition blc(2,0);
      IPosition trc(2,1,31*31-1);
      for (int i=0; i<100000; i++) {
	median(arr(blc,trc));
      }
    }
    timer.show();
    timer.mark();
    {
      IPosition blc(2,0);
      IPosition trc(2,1,31*31-1);
      for (int i=0; i<100000; i++) {
	smartMedian(arr(blc,trc));
      }
    }
    timer.show();
    timer.mark();
    {
      IPosition blc(2,0);
      IPosition trc(2,31*31-1,1);
      for (int i=0; i<100000; i++) {
	median(arr(blc,trc));
      }
    }
    timer.show();
    timer.mark();
    {
      IPosition blc(2,0);
      IPosition trc(2,31*31-1,1);
      for (int i=0; i<100000; i++) {
	smartMedian(arr(blc,trc));
      }
    }
    timer.show();
  }
}

void doItMasked (bool)
{
  {
    IPosition shape(2,5,5);
    Array<float> darr(shape);
    indgen(darr);
    MaskedArray<float> arr = darr(darr<=float(10) || darr>float(14));
    cout << arr.getMask() << endl;
    cout << sum(arr) << endl;
    cout << slidingArrayMath(arr, IPosition(2,2), MaskedSumFunc<float>(), false) << endl;
    cout << slidingArrayMath(arr, IPosition(2,1), MaskedSumFunc<float>(), false) << endl;

    cout << slidingArrayMath(arr, IPosition(4,2), MaskedSumFunc<float>(), true) << endl;
    cout << slidingArrayMath(arr, IPosition(3,1), MaskedSumFunc<float>(), true) << endl;

    cout << slidingArrayMath(arr, IPosition(2,2), MaskedMedianFunc<float>(), false) << endl;
    cout << slidingArrayMath(arr, IPosition(2,1), MaskedMedianFunc<float>(), false) << endl;
  }
}

int main(int argc, const char*[])
{
  try {
    doIt (argc>1);
    doItMasked (argc>1);
  } catch (exception& x) {
    cout << "Unexpected exception: " << x.what() << endl;
    return 1;
  }
  return 0;
}
