cmake_minimum_required(VERSION 2.8.8)

project(array2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wsuggest-override")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
find_package(Casacore REQUIRED COMPONENTS casa)
include_directories(${CASACORE_INCLUDE_DIRS})

find_package(Boost COMPONENTS filesystem unit_test_framework system)
include_directories(${Boost_INCLUDE_DIR})

add_library(array2-object OBJECT
  Array2.cc
  Array2Math.cc
  ArrayBase.cc
  ArrayError.cc
  Array_tmpl.cc
  ArrayOpsDiffShapes.cc
  ArrayPartMath.cc
  ArrayPosIter.cc
  ArrayUtil2.cc
  AxesMapping.cc
  AxesSpecifier.cc
  IPosition.cc
  Matrix_tmpl.cc
  Slice.cc
  Slicer.cc
  Vector_tmpl.cc)
set(ARRAY2FILES $<TARGET_OBJECTS:array2-object>)

add_executable(tArray2
  test/tArray.cc
  test/tArrayAccessor.cc
  test/tArrayBase.cc
  test/tArrayIO1.cc
  test/tArrayIO2.cc
  test/tArrayIO3.cc
  test/tArrayIter.cc
  test/tArrayIter1.cc
  test/tArrayIteratorSTL.cc
  test/tArrayLogical.cc
  test/tArrayMath.cc
  test/tArrayMathTransform.cc
  test/tArrayOpsDiffShapes.cc
#  test/tArrayPartMath.cc # it's so slow!
  test/tArrayPosIter.cc
  test/tArrayUtil.cc
  test/tAxesSpecifier.cc
  test/tBoxedArrayMath.cc
  test/tConvertArray.cc
  test/tCpp11Features.cc
  test/tSlice.cc
  test/tStringArray.cc
  test/tVector.cc
  ${ARRAY2FILES})
target_link_libraries(tArray2 ${CASACORE_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

message(STATUS "Flags passed to C++ compiler: " ${CMAKE_CXX_FLAGS})
