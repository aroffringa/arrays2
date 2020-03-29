#ifndef ARRAY2_ARRAY_FWD_H
#define ARRAY2_ARRAY_FWD_H

#include <memory>

namespace casacore { //#Begin casa namespace
namespace array2 {

template<typename T, typename Alloc = std::allocator<T>> class Array;
template<typename T, typename Alloc = std::allocator<T>> class Vector;
template<typename T, typename Alloc = std::allocator<T>> class Matrix;
template<typename T, typename Alloc = std::allocator<T>> class Cube;

} }

#endif
