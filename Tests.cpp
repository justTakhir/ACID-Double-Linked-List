#include <gtest/gtest.h>

#include <algorithm>

#include <ConsistentList.hpp>

TEST(InsertTest, Test1) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;

  list.insert(list.begin(), 1);
  std_list.insert(std_list.begin(), 1);
  auto it = std::find(list.begin(), list.end(), 1);

  auto it1 = it;

  ASSERT_TRUE(it != list.end());
  ASSERT_TRUE(*it == *it1 == 1);

  auto it2 = list.insert(list.begin().next(), 3, 7);
  auto std_it2 = std_list.insert(std::next(std_list.begin()), 3, 7);
  ASSERT_TRUE(*it2 == *std_it2);
  ASSERT_TRUE(*it2 == 7);

  std::initializer_list<double_t> init_list = { 2, 3, 4, 5 };
  auto it3 = list.insert(list.end().prev(), init_list);
  auto std_it3 = std_list.insert(std::prev(std_list.end()), init_list);

  ASSERT_TRUE(*it3 == *std_it3);
  ASSERT_TRUE(*it3 == 2);

  auto it4 = list.insert(it3.next(), 0);
  auto std_it4 = std_list.insert(std::next(std_it3), 0);
  ASSERT_TRUE(*it4 == *std_it4);
  ASSERT_TRUE(*it4 == 0);

  //auto it5 = list.insert(list.cend(), it3, it4);
  //auto std_it5 = std_list.insert(std_list.end(), std_it3, std_it4);
  ASSERT_TRUE(list == std_list);

}
