#include "../Array.h"
#include "../ArrayIO.h"
#include "../Slice.h"
#include "../Slicer.h"
#include "../Vector.h"

#include <boost/test/unit_test.hpp>

using namespace casacore::array2;

BOOST_AUTO_TEST_SUITE(cpp11_features)

void checkEmpty(const IPosition& ip)
{
  BOOST_CHECK_EQUAL(ip.empty(), true);
  BOOST_CHECK_EQUAL(ip.size(), 0);
}

void checkSmall(const IPosition& ip)
{
  BOOST_CHECK_EQUAL(ip.empty(), false);
  BOOST_CHECK_EQUAL(ip.size(), 2);
  BOOST_CHECK_EQUAL(ip[0], 2);
  BOOST_CHECK_EQUAL(ip[1], 3);
}

void checkLarge(const IPosition& ip)
{
  BOOST_CHECK_EQUAL(ip.empty(), false);
  BOOST_CHECK_EQUAL(ip.size(), 6);
  for(size_t i=0; i!=6; ++i)
    BOOST_CHECK_EQUAL(ip[i], i+2);
}

BOOST_AUTO_TEST_CASE( ip_initializerlist )
{
  IPosition ip{};
  checkEmpty(ip);
  
  IPosition ip1{2, 3};
  checkSmall(ip1);
  
  IPosition ip2{2, 3, 4, 5, 6, 7};
  checkLarge(ip2);
}

BOOST_AUTO_TEST_CASE( ip_move_constructor )
{
  IPosition ip1{2, 3};
  IPosition ip2(std::move(ip1));
  checkSmall(ip2);
  checkEmpty(ip1);
  
  IPosition ip3{2, 3, 4, 5, 6, 7};
  IPosition ip4(std::move(ip3));
  checkLarge(ip4);
  checkEmpty(ip3);
}

BOOST_AUTO_TEST_CASE( ip_move_assignment )
{
  IPosition ip1{2, 3}, ip2;
  ip2 = std::move(ip1);
  checkSmall(ip2);
  checkEmpty(ip1);
  
  IPosition ip3{2, 3, 4, 5, 6, 7}, ip4;
  ip4 = std::move(ip3);
  checkLarge(ip4);
  checkEmpty(ip3);
  
  ip4 = std::move(ip2);
  checkSmall(ip4);
  checkEmpty(ip2);
}

BOOST_AUTO_TEST_CASE( ip_non_conforming_copy_assignment )
{
  // Since the 2020 changes, this should no longer throw.
  IPosition ip1{2, 3};
  IPosition ip2{2, 3, 4, 5, 6, 7};
  ip1 = ip2; // normal copy assignment
  checkLarge(ip1);
  checkLarge(ip2);
}

BOOST_AUTO_TEST_CASE( array_move_constructor )
{
  IPosition ip{2, 3};
  Array<int> a(ip, 42);
  Array<int> b(std::move(a));
  Array<int> ref(ip, 42);
  BOOST_CHECK_EQUAL(b.shape(), ip);
  BOOST_CHECK_EQUAL(b.shape(), ip);
  BOOST_CHECK_EQUAL_COLLECTIONS(b.begin(), b.end(), ref.begin(), ref.end());
  Array<int> c(std::move(a));
  Array<int> empty;
  BOOST_CHECK_EQUAL_COLLECTIONS(c.begin(), c.end(), empty.begin(), empty.end());
}

BOOST_AUTO_TEST_CASE( array_iterate_empty )
{
  Array<int> empty1, empty2;
  BOOST_CHECK_EQUAL_COLLECTIONS(empty1.begin(), empty1.end(), empty2.begin(), empty2.end());
}

BOOST_AUTO_TEST_CASE( array_move_assignment )
{
  IPosition ip{2, 3};
  Array<int> a(ip, 42);
  Array<int> b;
  b = std::move(a);
  Array<int> ref(ip, 42);
  BOOST_CHECK_EQUAL(b.shape(), ip);
  BOOST_CHECK_EQUAL(b.shape(), ip);
  BOOST_CHECK_EQUAL_COLLECTIONS(b.begin(), b.end(), ref.begin(), ref.end());
}

BOOST_AUTO_TEST_CASE( array_swap )
{
  IPosition sa{2,3}, sb{4,4,4};
  Array<int> a(sa, 8), b(sb, 1982), c;
  casacore::array2::swap(a, b);
  casacore::array2::swap(b, c);
  BOOST_CHECK_EQUAL(a.shape(), sb);
  BOOST_CHECK(allEQ(a, 1982));
  BOOST_CHECK(b.empty());
  BOOST_CHECK_EQUAL(c.shape(), sa);
  BOOST_CHECK(allEQ(c, 8));
  std::swap(a, c);
  BOOST_CHECK_EQUAL(a.shape(), sa);
  BOOST_CHECK(allEQ(a, 8));
}

BOOST_AUTO_TEST_CASE( array_can_hold_noncopyable_objects )
{
  // Because Array now moves objects when possible, it can also store
  // non-copyable objects such as std::unique_ptr
  Array<std::unique_ptr<int>> a, b, c;
  //a = Array<std::unique_ptr<int>>(IPosition{2,3}, nullptr); // TODO this could be made possible but isn't yet
  a = Array<std::unique_ptr<int>>(IPosition{2,3});
  IPosition pos{1, 2};
  a(pos) = std::unique_ptr<int>(new int(42));
  b = std::move(a);
  BOOST_CHECK_EQUAL(*b(pos), 42);
  c.reference(b);
  BOOST_CHECK_EQUAL(*c(pos), 42);
  BOOST_CHECK_THROW(a.assign(b), ArrayError);
}

BOOST_AUTO_TEST_CASE( vector_can_hold_noncopyable_objects )
{
  Vector<std::unique_ptr<int>> a, b, c;
  a = Vector<std::unique_ptr<int>>(2);
  a[1] = std::unique_ptr<int>(new int(42));
  b = std::move(a);
  BOOST_CHECK_EQUAL(*b[1], 42);
  c.reference(b);
  BOOST_CHECK_EQUAL(*c[1], 42);
  BOOST_CHECK_THROW(a.assign(b), ArrayError);
}

BOOST_AUTO_TEST_CASE( array_to_string )
{
  Array<int> a(IPosition{2, 3}, 2);
  a(IPosition{0, 1}) = 1;
  BOOST_CHECK_EQUAL(to_string(a),
    "Axis Lengths: [2, 3]  (NB: Matrix in Row/Column order)\n"
    "[2, 1, 2\n"
    " 2, 2, 2]\n");
}

BOOST_AUTO_TEST_CASE( masked_array_move_constructor )
{
  Array<int> arr(IPosition{2, 3}, 37);
  LogicalArray mask(IPosition{2, 3}, false);
  mask(IPosition{0, 1}) = true;
  MaskedArray<int> ma1(arr, mask);
  MaskedArray<int> ma2(std::move(ma1));

  std::vector<int> arrReference{37, 37, 37, 37, 37, 37};
  BOOST_CHECK_EQUAL_COLLECTIONS(ma2.getArray().begin(), ma2.getArray().end(), arrReference.begin(), arrReference.end());
  
  std::vector<bool> maskReference{false, false, true, false, false, false};
  BOOST_CHECK_EQUAL_COLLECTIONS(ma2.getMask().begin(), ma2.getMask().end(), maskReference.begin(), maskReference.end());
}

BOOST_AUTO_TEST_CASE( masked_array_move_assignment )
{
  Array<int> arr(IPosition{2, 3}, 37);
  LogicalArray mask(IPosition{2, 3}, false);
  mask(IPosition{0, 1}) = true;
  MaskedArray<int> ma1(arr, mask), ma2, ma3;
  ma2 = ma1;
  ma3 = std::move(ma1);

  BOOST_CHECK_EQUAL_COLLECTIONS(ma2.getArray().begin(), ma2.getArray().end(), ma3.getArray().begin(), ma3.getArray().end());
  BOOST_CHECK_EQUAL_COLLECTIONS(ma2.getMask().begin(), ma2.getMask().end(), ma3.getMask().begin(), ma3.getMask().end());
}

BOOST_AUTO_TEST_CASE( masked_array_assign_rvalue_array )
{
  MaskedArray<int> ma;
  IPosition ip{2, 3};
  ma = Array<int>(ip, 37);
  BOOST_CHECK_EQUAL(ma.shape(), ip);
  for(size_t i=0; i!=6; ++i) {
    BOOST_CHECK_EQUAL(ma.getArray()(IPosition(2, i/3, i%3)), 37);
    BOOST_CHECK_EQUAL(ma.getMask()(IPosition(2, i/3, i%3)), true);
  }
}

BOOST_AUTO_TEST_SUITE_END()
