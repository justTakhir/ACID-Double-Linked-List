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
  ConsistentList<int32_t> list = {1, 3, 5};
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
/////////////////////////////////////////////////////////
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
/////////////////////////////////////////////

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
  std::random_device rd;
  std::mt19937 mersenne(rd());

  ConsistentList<int32_t> list;
  std::list<int32_t> std_list;
  
  for (size_t i = 0; i < 10; i++) {
    int32_t new_node = mersenne();
    list.push_front(new_node);
    std_list.push_front(new_node);
  }

  ASSERT_TRUE(list == std_list);
}

TEST(InsertTest, PushBack) {
  std::random_device rd;
  std::mt19937 mersenne(rd());

  ConsistentList<double_t> list;
  std::list<double_t> std_list;

  for (size_t i = 0; i < 10; i++) {
    int32_t new_node = mersenne();
    list.push_back(new_node);
    std_list.push_back(new_node);
  }

  ASSERT_TRUE(list == std_list);
}
/////////////////////////////////////////////
TEST(DeleteTest, AllEraseTest) {
  ConsistentList<double_t> list = { 1, 2, 3, 4, 5 };
  std::list<double_t> std_list = { 1, 2, 3, 4, 5};
  auto it3 = list.begin().next().next();
  auto std_it3 = std::next(std_list.begin(), 2);
  auto it4 = list.erase(it3);
  auto std_it4 = std_list.erase(std_it3);
  ASSERT_TRUE(list == std_list);
  ASSERT_TRUE(*it4 == *std_it4);

  list.erase(list.begin(), it4);
  std_list.erase(std_list.begin(), std_it4);
  ASSERT_TRUE(list == std_list);
  
}

TEST(DeleteTest, EraseEnd) {
  ConsistentList<double_t> list = { 1, 2, 3, 4, 5 };
  ASSERT_THROW(list.erase(list.end()), EraseException);
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
  std::random_device rd;
  std::mt19937 mersenne(rd());
  size_t lists_size = mersenne() / 5000000;
  ConsistentList<uint64_t> list(lists_size);
  std::list<uint64_t> std_list(lists_size);
  ASSERT_TRUE(list == std_list);

  for (auto it = list.begin(); it != list.end(); it++) {
    uint64_t new_node = mersenne();
    list.push_front(new_node);
    std_list.push_front(new_node);
    ASSERT_TRUE(list == std_list);
  }

  list.clear();
  std_list.clear();
  ASSERT_TRUE(list == std_list);
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
  size_t lists_size = my_rand();
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
  std::random_device rd;
  std::mt19937 mersenne(rd());
  size_t lists_size = mersenne() / 5000000;
  ConsistentList<float_t> list(lists_size);
  std::list<float_t> std_list(lists_size);
  ASSERT_TRUE(list == std_list);

  for (auto it = list.begin(); it != list.end(); it++) {
    float_t new_node = mersenne();
    list.push_front(new_node);
    std_list.push_front(new_node);
    ASSERT_TRUE(list == std_list);
  }
  //<
  list.resize(lists_size / 2);
  std_list.resize(lists_size / 2);
  ASSERT_TRUE(list == std_list);
  //>
  list.resize(lists_size * 3);
  std_list.resize(lists_size * 3);
  ASSERT_TRUE(list == std_list);
  //==
  list.resize(lists_size * 3);
  std_list.resize(lists_size * 3);
  ASSERT_TRUE(list == std_list);
}

TEST(ResizeTest, ResizeWithValue) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  size_t lists_size = mersenne() / 5000000;
  ConsistentList<double_t> list(lists_size);
  std::list<double_t> std_list(lists_size);
  ASSERT_TRUE(list == std_list);

  for (auto it = list.begin(); it != list.end(); it++) {
    double_t new_node = mersenne();
    list.push_front(new_node);
    std_list.push_front(new_node);
    ASSERT_TRUE(list == std_list);
  }
  //<
  double_t new_node = mersenne();
  list.resize(lists_size / 2, new_node);
  std_list.resize(lists_size / 2, new_node);
  ASSERT_TRUE(list == std_list);
  //>
  new_node = mersenne();
  list.resize(lists_size * 3, new_node);
  std_list.resize(lists_size * 3, new_node);
  ASSERT_TRUE(list == std_list);
  //==
  new_node = mersenne();
  list.resize(lists_size * 3, new_node);
  std_list.resize(lists_size * 3, new_node);
  ASSERT_TRUE(list == std_list);
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

TEST(MergeTest, MergeUnorderedLists1) {
  ConsistentList<int32_t> list = {3, 2};
  ConsistentList<int32_t> other_list(3, 4);
  ASSERT_THROW(list.merge(other_list), MergeException);
}

TEST(MergeTest, MergeUnorderedLists2) {
  ConsistentList<int32_t> list = { 1, 2, 3 };
  ConsistentList<int32_t> other_list = { 7, 6 };
  ASSERT_THROW(list.merge(other_list), MergeException);
}

TEST(ReverseTest, test1) {
  ConsistentList<int32_t> list1 = { 1, 3 ,5 ,7 };
  std::list<int32_t> std_list1 = { 1, 3 ,5 ,7 };

  list1.reverse();
  std_list1.reverse();
  ASSERT_TRUE(list1 == std_list1);
}

TEST(ReverseTest, Reverse) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  size_t lists_size = 100;

  ConsistentList<int32_t> list1;
  std::list<int32_t> std_list1;
  for (size_t i = 0; i < lists_size; i++) {
    int32_t new_node = mersenne();
    list1.push_back(new_node);
    std_list1.push_back(new_node);
  }
  ASSERT_TRUE(list1 == std_list1);
  
  list1.reverse();
  std_list1.reverse();
  ASSERT_TRUE(list1 == std_list1);
}

TEST(ConsistencyTest, Test1) {
  ConsistentList<int32_t> list = { 1,2,3,4,5 };
  Iterator<int32_t> it1(list.begin());
  Iterator<int32_t> it3_1(it1.next().next());
  auto it3_2 = it3_1;
  list.erase(it3_1);
  auto it3_3 = it3_1;

  ASSERT_TRUE(*it3_1 == *it3_2);
  ASSERT_TRUE(*it3_1 == *it3_3);
  ASSERT_TRUE(*it3_2 == *it3_3);
  ASSERT_TRUE(*it3_1 == 3);
  
  it3_1++;
  ASSERT_TRUE(*it3_1 == 4);
  it3_1--;
  ASSERT_TRUE(*it3_1 == 2);

  list.erase(list.begin().next());

  it3_2--;
  ASSERT_TRUE(*it3_2 == 2);
  it3_2++;
  ASSERT_TRUE(*it3_2 == 4);
}
 

TEST(ConsistencyTest, Test2) {
  ConsistentList<int32_t> list = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  Iterator<int32_t> it1(list.begin());
  Iterator<int32_t> it3(it1.next().next());
  Iterator<int32_t> it4 = it3.next();
  Iterator<int32_t> it5 = it4.next();
  Iterator<int32_t> it6 = it5.next();
  Iterator<int32_t> it7 = it6.next();
  Iterator<int32_t> it_end = list.end();
  Iterator<int32_t> it9 = it_end.prev();

  std::vector<Iterator<int32_t>> iterators = {it1, it3, it4, it5, it6, it7, it9};

  std::vector<Iterator<int32_t>> iterators_to_erased;

  iterators_to_erased.push_back(list.erase(it1));
  ASSERT_TRUE(*it1 == 1);
  iterators_to_erased.push_back(list.erase(it3));
  ASSERT_TRUE(*it3 == 3);
  iterators_to_erased.push_back(list.erase(it5));
  ASSERT_TRUE(*it5 == 5);
  iterators_to_erased.push_back(list.erase(it4));
  ASSERT_TRUE(*it4 == 4);
  iterators_to_erased.push_back(list.erase(it7));
  ASSERT_TRUE(*it7 == 7);
  iterators_to_erased.push_back(list.erase(it6));
  ASSERT_TRUE(*it6 == 6);

  for (size_t i = 0; i < iterators.size(); i++) {
    while (iterators[i] != list.end()) {
      iterators[i]++;
    }
  }

  for (auto i = 0; i < iterators.size(); i++) {
    ASSERT_TRUE(iterators[i] == list.end());
  }

  std::list<int32_t> list_for_compare = { 2, 8, 9 };
  ASSERT_TRUE(list == list_for_compare);

  for (size_t i = 0; i < iterators_to_erased.size(); i++) {
    while (iterators_to_erased[i] != list.end()) {
      iterators_to_erased[i]++;
    }
  }

  for (auto i = 0; i < iterators_to_erased.size(); i++) {
    ASSERT_TRUE(iterators_to_erased[i] == list.end());
  }

  ///////////////////////
  for (size_t i = 0; i < iterators.size(); i++) {
    iterators[i]--;
    while (iterators[i] != list.end()) {
      iterators[i]--;
    }
    iterators[i]++;
  }

  for (auto i = 0; i < iterators.size(); i++) {
    ASSERT_TRUE(iterators[i] == list.begin());
    ASSERT_TRUE(*iterators[i] == 2);
  }

  for (size_t i = 0; i < iterators_to_erased.size(); i++) {
    iterators_to_erased[i]--;
    while (iterators_to_erased[i] != list.end()) {
      iterators_to_erased[i]--;
    }
    iterators_to_erased[i]++;
  }

  for (auto i = 0; i < iterators_to_erased.size(); i++) {
    ASSERT_TRUE(iterators_to_erased[i] == list.begin());
    ASSERT_TRUE(*iterators_to_erased[i] == 2);
  }
}