//# MaskedArray.cc: A templated N-D masked array class with variable origin.
//# Copyright (C) 1993,1994,1995,1996,1997,1999,2001,2005
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

#ifndef CASA_MASKEDARRAY_2_TCC
#define CASA_MASKEDARRAY_2_TCC

#include "MaskedArray.h"
#include "Array.h"
#include "ArrayLogical.h"
#include "Slicer.h"
#include "ArrayError.h"

#include <cassert>

namespace casacore { //# NAMESPACE CASACORE - BEGIN
namespace array2 {

template<class T> 
MaskedArray<T>::MaskedArray ()
  : pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false), 
    isRO (false)
{}

template<class T> void
MaskedArray<T>::setData (const Array<T> &data,
			 const LogicalArray &mask,
			 bool isReadOnly){
  if (data.shape() != mask.shape()) {
    throw (ArrayConformanceError(
          "MaskedArray<T>::setData(const Array<T> &,"
          " const LogicalArray &, bool)"
          " - arrays do not conform"));
  }
  if (pArray) {
    delete pArray; 
    pArray = 0;
  }
  pArray = new Array<T>(data);
  if (pMask) {
    delete pMask; 
    pMask = 0;
  }
  pMask = new LogicalArray (mask.copy());
  nelemValid = 0;
  nelemValidIsOK = false;
  isRO  = isReadOnly; 
  assert(ok());
}

template<class T> void
MaskedArray<T>::setData (const MaskedArray<T> & array,
			 bool isReadOnly){
  if (pArray) {
    delete pArray; 
    pArray = 0;
  }
  pArray = new Array<T>(array.getArray());
  if (pMask) {
    delete pMask;
    pMask = 0;
  }
  pMask = new LogicalArray(array.getMask().copy());
  nelemValid = 0;
  nelemValidIsOK = false;
  isRO  = isReadOnly; 
  
  assert(ok());
}

template<class T>
MaskedArray<T>::MaskedArray (const Array<T> &inarray,
                             const LogicalArray &inmask,
                             bool isreadonly)
: pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false),
  isRO (isreadonly)
{
//    if (! conform2 (inarray, inmask)) {
    if (inarray.shape() != inmask.shape()) {
        throw (ArrayConformanceError(
          "MaskedArray<T>::MaskedArray(const Array<T> &,"
          " const LogicalArray &, bool)"
          " - arrays do not conform"));
    }

    pArray = new Array<T> (inarray);
    pMask = new LogicalArray (inmask.shape());
    pMask->assign_conforming(inmask);

  assert(ok());

}


template<class T>
MaskedArray<T>::MaskedArray (const Array<T> &inarray,
                             const LogicalArray &inmask)
: pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false), isRO (false)
{
//    if (! conform2 (inarray, inmask)) {
    if (inarray.shape() !=  inmask.shape()) {
        throw (ArrayConformanceError(
          "MaskedArray<T>::MaskedArray(const Array<T> &,"
          " const LogicalArray &)"
          " - arrays do not conform"));
    }

    pArray = new Array<T> (inarray);
    pMask = new LogicalArray (inmask.shape());
    pMask->assign_conforming(inmask);

  assert(ok());

}


template<class T>
MaskedArray<T>::MaskedArray (const MaskedArray<T> &inarray,
                             const LogicalArray &inmask,
                             bool isreadonly)
: pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false),
  isRO ( (inarray.isRO || isreadonly))
{
//    if (! conform2 (inarray, inmask)) {
    if (inarray.shape() != inmask.shape()) {
        throw (ArrayConformanceError(
          "MaskedArray<T>::MaskedArray (const MaskedArray<T> &,"
          " const LogicalArray &, bool)"
          " - arrays do not conform"));
    }


    pArray = new Array<T> (inarray.getArray());
    pMask = new LogicalArray (inmask.shape());
    *pMask = (inmask && inarray.getMask());

  assert(ok());

}


template<class T>
MaskedArray<T>::MaskedArray (const MaskedArray<T> &inarray,
                             const LogicalArray &inmask)
: pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false),
  isRO (inarray.isRO)
{
//    if (! conform2 (inarray, inmask)) {
    if (inarray.shape() != inmask.shape()) {
        throw (ArrayConformanceError(
          "MaskedArray<T>::MaskedArray (const MaskedArray<T> &,"
          " const LogicalArray &)"
          " - arrays do not conform"));
    }


    pArray = new Array<T> (inarray.getArray());
    pMask = new LogicalArray (inmask.shape());
    *pMask = (inmask && inarray.getMask());

  assert(ok());

}


template<class T>
MaskedArray<T>::MaskedArray (const Array<T> &inarray,
                             const MaskedLogicalArray &inmask,
                             bool isreadonly)
: pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false),
  isRO (isreadonly)
{
//    if (! conform2 (inarray, inmask)) {
    if (inarray.shape() != inmask.shape()) {
        throw (ArrayConformanceError(
          "MaskedArray<T>::MaskedArray(const Array<T> &inarray,"
          " const MaskedLogicalArray &inmask, bool isreadonly)"
          " - arrays do not conform"));
    }

    pArray = new Array<T> (inarray);
    pMask = new LogicalArray (inarray.shape());
    pMask->assign_conforming(inmask.getArray() && inmask.getMask());

  assert(ok());

}


template<class T>
MaskedArray<T>::MaskedArray (const Array<T> &inarray,
                             const MaskedLogicalArray &inmask)
: pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false), isRO (false)
{
//    if (! conform2 (inarray, inmask)) {
    if (inarray.shape() != inmask.shape()) {
        throw (ArrayConformanceError(
          "MaskedArray<T>::MaskedArray(const Array<T> &inarray,"
          " const MaskedLogicalArray &inmask)"
          " - arrays do not conform"));
    }

    pArray = new Array<T> (inarray);
    pMask = new LogicalArray (inarray.shape());
    pMask->assign_conforming(inmask.getArray() && inmask.getMask());

  assert(ok());

}


template<class T>
MaskedArray<T>::MaskedArray (const MaskedArray<T> &inarray,
                             const MaskedLogicalArray &inmask)
: pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false),
  isRO (inarray.isRO)
{
//    if (! conform2 (inarray, inmask)) {
    if (inarray.shape() != inmask.shape()) {
        throw (ArrayConformanceError(
          "MaskedArray<T>::MaskedArray (const MaskedArray<T> &inarray,"
          " const MaskedLogicalArray &inmask)"
          " - arrays do not conform"));
    }


    pArray = new Array<T> (inarray.getArray());
    pMask = new LogicalArray (inarray.shape());
    *pMask = (inmask.getArray() && inmask.getMask() && inarray.getMask());

  assert(ok());

}


template<class T>
MaskedArray<T>::MaskedArray (const MaskedArray<T> &inarray,
                             const MaskedLogicalArray &inmask,
                             bool isreadonly)
: pArray (0), pMask (0), nelemValid (0), nelemValidIsOK (false),
  isRO ( (inarray.isRO || isreadonly))
{
//    if (! conform2 (inarray, inmask)) {
    if (inarray.shape() != inmask.shape()) {
        throw (ArrayConformanceError(
          "MaskedArray<T>::MaskedArray (const MaskedArray<T> &inarray,"
          " const MaskedLogicalArray &inmask, bool isreadonly)"
          " - arrays do not conform"));
    }


    pArray = new Array<T> (inarray.getArray());
    pMask = new LogicalArray (inarray.shape());
    *pMask = (inmask.getArray() && inmask.getMask() && inarray.getMask());

  assert(ok());

}


template<class T> MaskedArray<T>::MaskedArray(const MaskedArray<T> &other,
                                              bool isreadonly)
: pArray (0), pMask (0),
  nelemValid (other.nelemValid), nelemValidIsOK (other.nelemValidIsOK),
  isRO ( (other.isRO || isreadonly))
{

    pArray = new Array<T> (*(other.pArray));
    pMask = new LogicalArray (*(other.pMask));

  assert(ok());

}


template<class T> MaskedArray<T>::MaskedArray(const MaskedArray<T> &other)
: pArray (0), pMask (0),
  nelemValid (other.nelemValid), nelemValidIsOK (other.nelemValidIsOK),
  isRO (other.isRO)
{

    pArray = new Array<T> (*(other.pArray));
    pMask = new LogicalArray (*(other.pMask));

  assert(ok());

}


template<class T> MaskedArray<T>::~MaskedArray()
{
    if (pArray) {
        delete pArray;
    }
    if (pMask) {
        delete pMask;
    }
}


template<class T> MaskedArray<T> MaskedArray<T>::copy(bool isreadonly) const
{
    assert(ok());

    MaskedArray<T> retval (pArray->copy(), *pMask, isreadonly);
    retval.nelemValid = nelemValid;
    retval.nelemValidIsOK = nelemValidIsOK;

    return retval;
}


template<class T> MaskedArray<T> MaskedArray<T>::copy() const
{
  assert(ok());

    MaskedArray<T> retval (pArray->copy(), *pMask);
    retval.nelemValid = nelemValid;
    retval.nelemValidIsOK = nelemValidIsOK;

    return retval;
}


template<class T>
MaskedArray<T> MaskedArray<T>::operator() (const LogicalArray &mask) const
{
  assert(ok());

    MaskedArray<T> ret (*this, mask);
    return ret;
}


template<class T>
MaskedArray<T> MaskedArray<T>::operator()
    (const MaskedLogicalArray &mask) const
{
  assert(ok());

    MaskedArray<T> ret (*this, mask);
    return ret;
}


template<class T>
MaskedArray<T> MaskedArray<T>::operator() (const IPosition &start,
					   const IPosition &end)
{
  assert(ok());
    return MaskedArray<T> ((*pArray)(start,end), (*pMask)(start,end), isRO);
}

template<class T>
MaskedArray<T> MaskedArray<T>::operator() (const IPosition &start,
					   const IPosition &end,
					   const IPosition &inc)
{
  assert(ok());
    return MaskedArray<T> ((*pArray)(start,end,inc), (*pMask)(start,end,inc),
			   isRO);
}

template<class T>
MaskedArray<T> MaskedArray<T>::operator() (const Slicer &slicer)
{
  assert(ok());
    return MaskedArray<T> ((*pArray)(slicer), (*pMask)(slicer), isRO);
}


template<class T> const Array<T> & MaskedArray<T>::getArray() const
{
  assert(ok());

    return *pArray;
}


template<class T> const LogicalArray & MaskedArray<T>::getMask() const
{
    assert(ok());

    return *pMask;
}


template<class T> size_t MaskedArray<T>::ndim() const
{
    assert(ok());

    return pArray->ndim();
}


template<class T> size_t MaskedArray<T>::nelementsValid() const
{
    assert(ok());

    if (!nelemValidIsOK) {
        // Calculate nelemValid;

        bool maskDelete;
        const LogicalArrayElem *maskStorage = getMaskStorage(maskDelete);
        const LogicalArrayElem *maskS = maskStorage;

        size_t nelemValidTmp = 0;
        size_t ntotal = nelements();
        while (ntotal--) {
            if (*maskS) {
                nelemValidTmp++;
            }
            maskS++;
        }

        freeMaskStorage(maskStorage, maskDelete);

        MaskedArray<T> *nonconstThis = (MaskedArray<T> *) this;
        nonconstThis->nelemValid = nelemValidTmp;
        nonconstThis->nelemValidIsOK = true;
    }

    return nelemValid;
}


template<class T> size_t MaskedArray<T>::nelements() const
{
    assert(ok());

    return pArray->nelements();
}


template<class T> bool MaskedArray<T>::ok() const
{
  if (!pArray && !pMask) return true; // default constructed is ok
  if (!pArray || !pMask) return false; // not both set is not ok
  return (pArray->ok() && pMask->ok()) ? true : false;
}


template<class T> bool MaskedArray<T>::conform(const Array<T> &other) const
{
    assert(ok());

    return pArray->conform(other);
}


template<class T>
bool MaskedArray<T>::conform(const MaskedArray<T> &other) const
{
    assert(ok());

    return pArray->conform(*(other.pArray));
}


template<class T> void MaskedArray<T>::setReadOnly() const
{
    assert(ok());

    MaskedArray<T> *nonconstThis = (MaskedArray<T> *) this;
    nonconstThis->isRO = true;
}


template <class T>
Array<T> MaskedArray<T>::getCompressedArray () const
{
    Array<T> result (IPosition (1,nelementsValid()));

    bool deleteResult;
    T *resultStorage = result.getStorage (deleteResult);
    T *resultS = resultStorage;

    bool deleteArr;
    const T *arrStorage = getArrayStorage (deleteArr);
    const T *arrS = arrStorage;

    bool deleteMask;
    const LogicalArrayElem *maskStorage
        = getMaskStorage (deleteMask);
    const LogicalArrayElem *maskS = maskStorage;

    size_t ntotal = nelementsValid();
    while (ntotal) {
        if (*maskS) {
            *resultS = *arrS;
            resultS++;
            ntotal--;
        }
        maskS++;
        arrS++;
    }

    result.putStorage (resultStorage, deleteResult);
    freeArrayStorage (arrStorage, deleteArr);
    freeMaskStorage (maskStorage, deleteMask);

    return result;
}


template <class T>
Array<T> MaskedArray<T>::getCompressedArray (const IPosition & shape) const
{
    if (int(nelementsValid()) != shape.product()) {
        throw (ArrayError
        ("void MaskedArray<T>::getCompressedArray (const IPosition & shape)"
         " - input shape will create Array with incorrect number of elements"));

    }

    Array<T> result (shape);

    bool deleteResult;
    T *resultStorage = result.getStorage (deleteResult);
    T *resultS = resultStorage;

    bool deleteArr;
    const T *arrStorage = getArrayStorage (deleteArr);
    const T *arrS = arrStorage;

    bool deleteMask;
    const LogicalArrayElem *maskStorage
        = getMaskStorage (deleteMask);
    const LogicalArrayElem *maskS = maskStorage;

    size_t ntotal = nelementsValid();
    while (ntotal) {
        if (*maskS) {
            *resultS = *arrS;
            resultS++;
            ntotal--;
        }
        maskS++;
        arrS++;
    }

    result.putStorage (resultStorage, deleteResult);
    freeArrayStorage (arrStorage, deleteArr);
    freeMaskStorage (maskStorage, deleteMask);

    return result;
}


template <class T>
void MaskedArray<T>::getCompressedArray (Array<T> & inarr) const
{
    if (nelementsValid() != inarr.nelements()) {
        throw (ArrayError
            ("void MaskedArray<T>::getCompressedArray (Array<T> & inarr)"
             " - input Array number of elements is incorrect"));

    }

    bool deleteInarr;
    T *inarrStorage = inarr.getStorage (deleteInarr);
    T *inarrS = inarrStorage;

    bool deleteArr;
    const T *arrStorage = getArrayStorage (deleteArr);
    const T *arrS = arrStorage;

    bool deleteMask;
    const LogicalArrayElem *maskStorage
        = getMaskStorage (deleteMask);
    const LogicalArrayElem *maskS = maskStorage;

    size_t ntotal = nelementsValid();
    while (ntotal) {
        if (*maskS) {
            *inarrS = *arrS;
            inarrS++;
            ntotal--;
        }
        maskS++;
        arrS++;
    }

    inarr.putStorage (inarrStorage, deleteInarr);
    freeArrayStorage (arrStorage, deleteArr);
    freeMaskStorage (maskStorage, deleteMask);
}


template <class T>
void MaskedArray<T>::setCompressedArray (const Array<T> & inarr)
{
    if (nelementsValid() != inarr.nelements()) {
        throw (ArrayError
            ("void MaskedArray<T>::setCompressedArray (const Array<T> & inarr)"
             " - input array number of elements is incorrect"));

    }

    bool deleteInarr;
    const T *inarrStorage = inarr.getStorage (deleteInarr);
    const T *inarrS = inarrStorage;

    bool deleteArr;
    T *arrStorage = getRWArrayStorage (deleteArr);
    T *arrS = arrStorage;

    bool deleteMask;
    const LogicalArrayElem *maskStorage
        = getMaskStorage (deleteMask);
    const LogicalArrayElem *maskS = maskStorage;

    size_t ntotal = nelementsValid();
    while (ntotal) {
        if (*maskS) {
            *arrS = *inarrS;
            inarrS++;
            ntotal--;
        }
        maskS++;
        arrS++;
    }

    inarr.freeStorage (inarrStorage, deleteInarr);
    putArrayStorage (arrStorage, deleteArr);
    freeMaskStorage (maskStorage, deleteMask);
}


template<class T>
const T * MaskedArray<T>::getArrayStorage (bool &deleteIt) const
{
    assert(ok());

    return pArray->getStorage (deleteIt);
}


template<class T>
void MaskedArray<T>::freeArrayStorage(const T *&storage, bool deleteIt) const
{
    assert(ok());

    pArray->freeStorage (storage, deleteIt);
}


template<class T>
const LogicalArrayElem * MaskedArray<T>::getMaskStorage (bool &deleteIt) const
{
    assert(ok());

    return pMask->getStorage (deleteIt);
}


template<class T> void  MaskedArray<T>::freeMaskStorage
    (const LogicalArrayElem *&storage, bool deleteIt) const
{
    assert(ok());

    pMask->freeStorage (storage, deleteIt);
}



template<class T> MaskedArray<T> & MaskedArray<T>::operator=
                                                   (const Array<T> &inarray)
{
    assert(ok());

    if (!pArray) {
      LogicalArray mask(inarray.shape());
      mask = true;
      setData(inarray,mask);
      return *this;
    }

    if (!conform(inarray)) {
            throw(ArrayConformanceError(
             "MaskedArray<T> & MaskedArray<T>::operator= "
             "(const Array<T> &inarray)"
             "- Conformance error."));
    }

    if (isRO) {
            throw(ArrayError(
             "MaskedArray<T> & MaskedArray<T>::operator= "
             "(const Array<T> &inarray)"
             "- this is read only."));
    }

    bool deleteArr;
    T *arrStorage = getRWArrayStorage(deleteArr);
    T *arrS = arrStorage;

    bool deleteMask;
    const LogicalArrayElem *maskStorage = getMaskStorage(deleteMask);
    const LogicalArrayElem *maskS = maskStorage;

    bool deleteInarr;
    const T *inarrStorage = inarray.getStorage(deleteInarr);
    const T *inarrS = inarrStorage;

    size_t ntotal = pArray->nelements();
    while (ntotal--) {
        if (*maskS) {
            *arrS = *inarrS;
        }
        arrS++;
        maskS++;
        inarrS++;
    }

    putArrayStorage(arrStorage, deleteArr);
    freeMaskStorage(maskStorage, deleteMask);
    inarray.freeStorage(inarrStorage, deleteInarr);

    return *this;
}

template<class T>
MaskedArray<T> &MaskedArray<T>::operator= (const MaskedArray<T> &other)
{
    assert(ok());

    if (this == &other)
	return *this;

    if (!pArray) {
      setData(other.copy());
      return *this;
    }

    if (!conform(other)) {
        throw(ArrayConformanceError(
            "MaskedArray<T> & MaskedArray<T>::operator= "
             "(const MaskedArray<T> &other)"
             "- Conformance error."));
    }

    if (isRO) {
        throw(ArrayError(
            "MaskedArray<T> & MaskedArray<T>::operator= "
             "(const MaskedArray<T> &other)"
             "- this is read only."));
    }

    bool deleteArr;
    T *arrStorage = getRWArrayStorage(deleteArr);
    T *arrS = arrStorage;

    bool deleteMask;
    const LogicalArrayElem *maskStorage = getMaskStorage(deleteMask);
    const LogicalArrayElem *maskS = maskStorage;

    bool deleteOarr;
    const T *oarrStorage = other.getArrayStorage(deleteOarr);
    const T *oarrS = oarrStorage;

    bool deleteOmask;
    const LogicalArrayElem *omaskStorage =
        other.getMaskStorage(deleteOmask);
    const LogicalArrayElem *omaskS = omaskStorage;

    size_t ntotal = pArray->nelements();
    while (ntotal--) {
        if (*maskS && *omaskS) {
            *arrS = *oarrS;
        }
        arrS++;
        maskS++;
        oarrS++;
        omaskS++;
    }

    putArrayStorage(arrStorage, deleteArr);
    freeMaskStorage(maskStorage, deleteMask);
    other.freeArrayStorage(oarrStorage, deleteOarr);
    other.freeMaskStorage(omaskStorage, deleteOmask);

    return *this;
}

template<class T> MaskedArray<T> &MaskedArray<T>::operator=(const T &val)
{
    assert(ok());
    if (!pArray) return *this;

    if (isRO) {
        throw(ArrayError(
            "MaskedArray<T> & MaskedArray<T>::operator= (const T &val)"
             "- this is read only."));
    }

    bool deleteArr;
    T *arrStorage = getRWArrayStorage(deleteArr);
    T *arrS = arrStorage;

    bool deleteMask;
    const LogicalArrayElem *maskStorage = getMaskStorage(deleteMask);
    const LogicalArrayElem *maskS = maskStorage;

    size_t ntotal = pArray->nelements();
    while (ntotal--) {
        if (*maskS) {
            *arrS = val;
        }
        arrS++;
        maskS++;
    }

    putArrayStorage(arrStorage, deleteArr);
    freeMaskStorage(maskStorage, deleteMask);

    return *this;
}


template<class T>
T * MaskedArray<T>::getRWArrayStorage (bool &deleteIt) const
{
    assert(ok());

    if (isRO) {
        throw(ArrayError(
            "MaskedArray<T>::getRWArrayStorage (bool &deleteIt) const"
             "- this is read only."));
    }

    return pArray->getStorage (deleteIt);
}


template<class T>
void MaskedArray<T>::putArrayStorage(T *&storage, bool deleteAndCopy) const
{
    assert(ok());

    if (isRO) {
        throw(ArrayError(
            "MaskedArray<T>::putArrayStorage (bool deleteAndCopy) const"
             "- this is read only."));
    }

    pArray->putStorage (storage, deleteAndCopy);
}


template<class T> Array<T> & MaskedArray<T>::getRWArray() const
{
    assert(ok());

    if (isRO) {
        throw(ArrayError(
            "Array<T> & MaskedArray<T>::getRWArray () const"
             "- this is read only."));
    }

    return *pArray;
}


//# Global functions.

template<class T, class U>
  bool conform2 (const MaskedArray<T> &left, const Array<U> &right)
{

    IPosition leftShape (left.shape());
    IPosition rightShape (right.shape());

    return ( (leftShape.conform (rightShape)) && (leftShape == rightShape) )
           ? true : false;
}

template<class T, class U>
  bool conform2 (const Array<T> &left, const MaskedArray<U> &right)
{

    IPosition leftShape (left.shape());
    IPosition rightShape (right.shape());

    return ( (leftShape.conform (rightShape)) && (leftShape == rightShape) )
           ? true : false;
}

template<class T, class U>
  bool conform2 (const MaskedArray<T> &left, const MaskedArray<U> &right)
{

    IPosition leftShape (left.shape());
    IPosition rightShape (right.shape());

    return ( (leftShape.conform (rightShape)) && (leftShape == rightShape) )
           ? true : false;
}


} } //# NAMESPACE CASACORE - END


#endif
