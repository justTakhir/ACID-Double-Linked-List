#include <gtest/gtest.h>

#include <algorithm>

#include "ConsistentList.hpp"

TEST(InsertTest, Test1) {
  ConsistentList<int> list;
  list.insert(list.begin(), 1);

  auto it = std::find(list.begin(), list.end(), 1);

  ASSERT_TRUE(it != list.end());
  ASSERT_TRUE(*it == 1);
}
