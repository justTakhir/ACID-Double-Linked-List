#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include <ConsistentList.hpp>

static int my_rand(int min = 1, int max = 33676) {
  static std::random_device rd;
  static std::mt19937 mersenne(rd());

  return min + mersenne() % (max - min);
}

TEST(IteratorTest, IteratorDereferencing) {
  ConsistentList<int32_t> list = { 1, 3, 5 };
  Iterator<int32_t> it = list.begin();
  ASSERT_TRUE(*it == 1);
  it--;

  ASSERT_THROW(*it, IteratorDereferencingException);
  ASSERT_THROW(*list.end(), IteratorDereferencingException);
}

TEST(IteratorTest, IteratorDecriment1) {
  ConsistentList<int32_t> list = { 1, 3, 5 };
  Iterator<int32_t> it = list.begin();

  it--;
  ASSERT_TRUE(it == list.end());

  it--;
  ASSERT_TRUE(*it == 5);

  --it;
  ASSERT_TRUE(*it == 3);

  --it;
  ASSERT_TRUE(*it == 1);
}

TEST(IteratorTest, IteratorPrefixDecriment2) {
  ConsistentList<int32_t> list = { 1, 3, 5 };
  Iterator<int32_t> it = list.begin();

  auto it1 = --it;
  ASSERT_TRUE(it1 == list.end());
  ASSERT_TRUE(it == list.end());
}

TEST(IteratorTest, IteratorPostfixDecriment2) {
  ConsistentList<int32_t> list = { 1, 3, 5 };
  Iterator<int32_t> it = list.begin();

  auto it1 = it--;
  ASSERT_TRUE(it1 == list.begin());
  ASSERT_TRUE(it == list.end());
}

TEST(IteratorTest, IteratorIncrement) {
  ConsistentList<int32_t> list = { 1, 3, 5 };
  Iterator<int32_t> it = list.begin();

  it++;
  ASSERT_TRUE(*it == 3);

  it++;
  ASSERT_TRUE(*it == 5);

  ++it;
  ASSERT_TRUE(it == list.end());

  ++it;
  ASSERT_TRUE(*it == 1);
  ASSERT_TRUE(it == list.begin());
}

TEST(IteratorTest, IteratorPrefixIncriment2) {
  ConsistentList<int32_t> list = { 1, 3, 5 };
  Iterator<int32_t> it = list.begin();

  auto it1 = ++it;
  ASSERT_TRUE(*it1 == 3);
  ASSERT_TRUE(*it == 3);
}

TEST(IteratorTest, IteratorPostfixIncriment2) {
  ConsistentList<int32_t> list = { 1, 3, 5 };
  Iterator<int32_t> it = list.begin();

  auto it1 = it++;
  ASSERT_TRUE(it1 == list.begin());
  ASSERT_TRUE(*it == 3);
}

TEST(IteratorTest, IteratorAddOperator) {
  ConsistentList list = { 1, 12, 3, 2, 43 };
  Iterator<int32_t> it = list.begin();
  it = it + 3;
  ASSERT_TRUE(*it == 2);
}

TEST(IteratorTest, IteratorSubOperator) {
  ConsistentList list = { 1, 12, 3, 2, 43 };
  auto it = list.end();
  it = it - 3;
  ASSERT_TRUE(*it == 3);
}