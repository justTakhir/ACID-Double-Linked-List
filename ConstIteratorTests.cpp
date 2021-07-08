#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include <ConsistentList.hpp>

static int my_rand(int min = 1, int max = 33676) {
  static std::random_device rd;
  static std::mt19937 mersenne(rd());

  return min + mersenne() % (max - min);
}

TEST(ConstIteratorTest, ConstIteratorDereferencing) {
  ConsistentList<int32_t> list = { 2, 4, 6 };
  ConstIterator<int32_t> it = list.begin();
  ASSERT_TRUE(*it == 2);
  it--;

  ASSERT_THROW(*it, IteratorDereferencingException);
  ASSERT_THROW(*list.end(), IteratorDereferencingException);
}

TEST(ConstIteratorTest, ConstIteratorDecriment) {
  ConsistentList<int32_t> list = { 2, 4, 6 };
  ConstIterator<int32_t> it = list.begin();

  it--;
  ASSERT_TRUE(it == list.end());

  it--;
  ASSERT_TRUE(*it == 6);

  --it;
  ASSERT_TRUE(*it == 4);

  --it;
  ASSERT_TRUE(*it == 2);
}

TEST(ConstIteratorTest, ConstIteratorPrefixDecriment2) {
  ConsistentList<int32_t> list = { 2, 4, 6 };
  Iterator<int32_t> it = list.begin();

  auto it1 = --it;
  ASSERT_TRUE(it1 == list.end());
  ASSERT_TRUE(it == list.end());
}

TEST(ConstIteratorTest, ConstIteratorPostfixDecriment2) {
  ConsistentList<int32_t> list = { 2, 4, 6 };
  Iterator<int32_t> it = list.begin();

  auto it1 = it--;
  ASSERT_TRUE(it1 == list.begin());
  ASSERT_TRUE(it == list.end());
}

TEST(ConstIteratorTest, ConstIteratorIncrement) {
  ConsistentList<int32_t> list = { 2, 4, 6 };
  ConstIterator<int32_t> it = list.begin();

  it++;
  ASSERT_TRUE(*it == 4);

  it++;
  ASSERT_TRUE(*it == 6);

  ++it;
  ASSERT_TRUE(it == list.end());

  ++it;
  ASSERT_TRUE(*it == 2);
  ASSERT_TRUE(it == list.begin());
}

TEST(ConstIteratorTest, ConstIteratorPrefixIncriment2) {
  ConsistentList<int32_t> list = { 2, 4, 6 };
  ConstIterator<int32_t> it = list.begin();

  auto it1 = ++it;
  ASSERT_TRUE(*it1 == 4);
  ASSERT_TRUE(*it == 4);
}

TEST(ConstIteratorTest, ConstIteratorPostfixIncriment2) {
  ConsistentList<int32_t> list = { 2, 4, 6 };
  ConstIterator<int32_t> it = list.begin();

  auto it1 = it++;
  ASSERT_TRUE(it1 == list.begin());
  ASSERT_TRUE(*it == 4);
}

TEST(ConstIteratorTest, ConstIteratorAddOperator) {
  ConsistentList<int32_t> list = { 1, 12, 3, 2, 43 };
  ConstIterator<int32_t> it = list.begin();
  it = it + 3;
  ASSERT_TRUE(*it == 2);
}

TEST(ConstIteratorTest, ConstIteratorSubOperator) {
  ConsistentList<int32_t> list = { 1, 12, 3, 2, 43 };
  ConstIterator<int32_t> it = list.end();
  it = it - 3;
  ASSERT_TRUE(*it == 3);
}

TEST(ConstIteratorTest, ConstIteratorAddOperatorWithoutEquating) {
  ConsistentList<int32_t> list = { 1, 12, 3, 2, 43 };
  ConstIterator<int32_t> it = list.begin();
  ConstIterator<int32_t> it2 = it + 3;
  ASSERT_TRUE(*it2 == 2);
  ASSERT_TRUE(*it == 1);
}

TEST(ConstIteratorTest, ConstIteratorSubOperatorWithoutEquating) {
  ConsistentList<int32_t> list = { 1, 12, 3, 2, 43 };
  ConstIterator<int32_t> it = list.begin();
  ConstIterator<int32_t> it2 = it - 3;
  ASSERT_TRUE(*it2 == 2);
  ASSERT_TRUE(*it == 1);
}