#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include <ConsistentList.hpp>

//no test for begin(), end()

TEST(InsertTest, AllInsert) {
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

  auto it5 = list.insert(list.end(), std_it2, std_it4);
  auto std_it5 = std_list.insert(std_list.end(), std_it2, std_it4);
  ASSERT_TRUE(list == std_list);

}

TEST(InsertTest, PushFront) {
  ConsistentList<int32_t> list;
  std::list<int32_t> std_list;
  
  for (size_t i = 0; i < 10; i++) {
    int32_t new_node = std::rand();
    list.push_front(new_node);
    std_list.push_front(new_node);
  }

  ASSERT_TRUE(list == std_list);
}

TEST(InsertTest, PushBack) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;

  for (size_t i = 0; i < 10; i++) {
    int32_t new_node = std::rand();
    list.push_back(new_node);
    std_list.push_back(new_node);
  }

  ASSERT_TRUE(list == std_list);
}
/////////////////////////////////////////////
TEST(DeleteTest, AllEraseTest) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;


}

TEST(DeleteTest, PopFront) {
  ConsistentList<int32_t> list;
  std::list<int32_t> std_list;

  for (size_t i = 0; i < 10; i++) {
    int32_t new_node = std::rand();
    list.push_front(new_node);
    std_list.push_front(new_node);
  }

  ASSERT_TRUE(list == std_list);

  for (size_t i = 0; i < 10; i++) {
    list.pop_front();
    std_list.pop_front();
    ASSERT_TRUE(list == std_list);
  }
}

TEST(DeleteTest, PopBack) {
  ConsistentList<int32_t> list;
  std::list<int32_t> std_list;

  for (size_t i = 0; i < 10; i++) {
    int32_t new_node = std::rand();
    list.push_front(new_node);
    std_list.push_front(new_node);
  }

  ASSERT_TRUE(list == std_list);

  for (size_t i = 0; i < 10; i++) {
    list.pop_back();
    std_list.pop_back();
    ASSERT_TRUE(list == std_list);
  }
}

TEST(DeleteTest, Clear) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;
}

TEST(DeleteTest, Remove) {
  ConsistentList<char> list;
  std::list<char> std_list;

  for (char i = 'a'; i < 'z'; i++) {
    for (uint32_t j = 'z' - i - 1; j > 0; j--) {
      list.push_front(i);
      std_list.push_front(i);
      ASSERT_TRUE(list == std_list);
    }
  }

  list.remove('a');
  std_list.remove('a');
  ASSERT_TRUE(list == std_list);

  list.remove('g');
  std_list.remove('g');
  ASSERT_TRUE(list == std_list);

  list.remove('x');
  std_list.remove('x');
  ASSERT_TRUE(list == std_list);

  list.remove('z');
  std_list.remove('z');
  ASSERT_TRUE(list == std_list);
}

TEST(SizeTest, Size) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  size_t lists_size =  mersenne() / 5000000;
  ConsistentList<uint32_t> list(lists_size);
  std::list<uint32_t> std_list(lists_size);
  ASSERT_TRUE(list == std_list);
  ASSERT_TRUE(list.size() == std_list.size());
  
  for (auto it = list.begin(); it != list.end(); it++) {
    uint32_t new_node = mersenne();
    list.push_front(new_node);
    std_list.push_front(new_node);
    ASSERT_TRUE(list == std_list);
    ASSERT_TRUE(list.size() == std_list.size());
  }
  
}

TEST(SizeTest, Empty) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  size_t lists_size = mersenne() / 50000000;
  ConsistentList<int16_t> list(lists_size);
  std::list<int16_t> std_list(lists_size);

  ASSERT_FALSE(list.empty());
  ASSERT_FALSE(std_list.empty());
  ASSERT_TRUE(list.empty() == std_list.empty());

  list.clear();
  std_list.clear();

  ASSERT_TRUE(list.empty());
  ASSERT_TRUE(std_list.empty());
  ASSERT_TRUE(list.empty() == std_list.empty());
}

TEST(ResizeTest, Resize) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;

  
}

TEST(ResizeTest, ResizeWithValue) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;
}

TEST(SwapTest, Swap) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;
}

TEST(MergeTest, Merge1) {
  ConsistentList<int8_t> list1(5, 3);
  std::list<int8_t> std_list1(5, 3);
  ASSERT_TRUE(list1 == std_list1);

  ConsistentList<int8_t> list2(5, 7);
  std::list<int8_t> std_list2(5, 7);
  ASSERT_TRUE(list2 == std_list2);

  list1.merge(list2);
  std_list1.merge(std_list2);
  ASSERT_TRUE(list1 == std_list1);

}

TEST(MergeTest, Merge2) {
  ConsistentList<int8_t> list1(4, 7);
  std::list<int8_t> std_list1(4, 7);
  ASSERT_TRUE(list1 == std_list1);

  ConsistentList<int8_t> list2(4, 2);
  std::list<int8_t> std_list2(4, 2);
  ASSERT_TRUE(list2 == std_list2);

  list1.merge(list2);
  std_list1.merge(std_list2);
  ASSERT_TRUE(list1 == std_list1);

}

TEST(MergeTest, MergeEqual) {
  ConsistentList<int8_t> list1(5, 4);
  std::list<int8_t> std_list1(5, 4);
  ASSERT_TRUE(list1 == std_list1);

  ConsistentList<int8_t> list2(5, 4);
  std::list<int8_t> std_list2(5, 4);
  ASSERT_TRUE(list2 == std_list2);

  list1.merge(list2);
  std_list1.merge(std_list2);
  ASSERT_TRUE(list1 == std_list1);

}

TEST(ReverseTest, Reverse) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;
}

TEST(ConsistencyTest, Test1) {
  ConsistentList<double_t> list;
  std::list<double_t> std_list;
}