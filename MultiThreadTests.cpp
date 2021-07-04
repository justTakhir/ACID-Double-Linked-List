#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <thread>


#include <ConsistentList.hpp>

TEST(InsertTests, AllInserts) {
  std::vector<std::thread> threads;
  ConsistentList<int32_t> list;
  std::list<int32_t> list_for_insert;
  list_for_insert = { 4, 4 };
  auto std_it1 = list_for_insert.begin();
  auto std_it2 = list_for_insert.end();

  for (size_t i = 0; i < 4; i++) {
    threads.push_back(std::thread([&] {
      list.insert(list.end(), i);
      list.insert(list.end(), {7, 7, 7});
      list.insert(list.end(), 2, 8);
      list.insert(list.end(), std_it1, std_it2);
      }));
    threads[i].join();
  }

  //for (auto it = list.begin(); it != list.end(); it++) {
  //  std::cout << *it << std::endl;
  //}

}

TEST(DeleteTests, PopFront) {
  ConsistentList<int32_t> list = {2, 3, 6};
  std::thread thr1([&] {
    list.pop_front();
    });

  std::thread thr2([&] {
    list.pop_front();
    });

  thr1.join();
  thr2.join();

  ASSERT_TRUE(list.size() == 1);
  ASSERT_TRUE(*list.begin() == 6);
}

TEST(DeleteTests, EraseInSameTime) {
  //from wiki
  ConsistentList<int32_t> list = { 1, 2, 3, 4, 5 };
  auto it2 = list.begin() + 2;
  auto it1 = it2++;

  std::thread th1([&] {
    list.erase(it1);
  });

  std::thread th2([&] {
    list.erase(it2);
  });

  th1.join();
  th2.join();

  ASSERT_TRUE(list.size() == 3);
}

TEST(DeleteTests, Erase) {
  ConsistentList<int32_t> list = {1, 2, 3, 4, 5};
  std::vector<std::thread> threads;

  for (auto it = list.begin(); it != list.end(); it++) {
    threads.push_back(std::thread([&] {
      list.erase(it);
      }));
    threads[(*it) - 1].join();
  }

  
  ASSERT_TRUE(list.empty());
}

TEST(IteratorsTests, AddAndWrite) {

  std::list<int32_t> l = { 1, 2, 3, 4, 5, 6, 7 };
  auto lit1 = l.begin();
  auto lit2 = std::next(lit1, 4);

  ConsistentList<int32_t> list = {1, 2, 3, 4, 5, 6, 7};
  auto it1 = list.begin();
  auto it2 = it1;
  it2 + 4;//TODO fix iterators +
  std::thread thr1([&] {
    it1 = it1 + 4;
    });
  std::thread thr2([&] {
    *it2 = 13;
    });
  thr2.join();
  ASSERT_TRUE(*it1 == 13);
  thr1.join();
  
  ASSERT_TRUE(*it1 == 13);
}

TEST(IteratorsTests, AddAndWrite2) {

}

TEST(IteratorsTests, SubAndWrite) {

}

TEST(IteratorsTests, SubAndWrite2) {

}