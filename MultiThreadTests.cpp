//#include <gtest/gtest.h>
//
//#include <algorithm>
//#include <random>
//#include <thread>
//#include <Windows.h>
//
//#include <ConsistentList.hpp>
//
//TEST(InsertTests, AllInserts) {
//  std::vector<std::thread> threads;
//  ConsistentList<int32_t> list;
//  std::list<int32_t> list_for_insert;
//  list_for_insert = { 4, 4 };
//  auto std_it1 = list_for_insert.begin();
//  auto std_it2 = list_for_insert.end();
//
//  for (size_t i = 0; i < 4; i++) {
//    threads.push_back(std::thread([&] {
//      if (i == 0) {
//        list.insert(list.end(), i);
//      }
//      if (i == 1) { 
//        list.insert(list.end(), { 7, 7, 7 }); 
//      }
//      if (i == 2) {
//        list.insert(list.end(), 2, 8);
//      }
//      if (i == 3) {
//        list.insert(list.end(), std_it1, std_it2);
//      }
//      }));
//    //threads[i].join();
//  }
//
//  for (size_t i = 0; i < 4; i++) {
//    threads[i].join();
//  }
//
//  //for (auto it = list.begin(); it != list.end(); it++) {
//  //  std::cout << *it << std::endl;
//  //}
//
//}
//
//TEST(DeleteTests, PopFront) {
//  ConsistentList<int32_t> list = {2, 3, 6};
//  std::thread thr1([&] {
//    list.pop_front();
//    });
//
//  std::thread thr2([&] {
//    list.pop_front();
//    });
//
//  thr1.join();
//  thr2.join();
//
//  ASSERT_TRUE(list.size() == 1);
//  ASSERT_TRUE(*list.begin() == 6);
//}
//
//TEST(DeleteTests, EraseInSameTime) {
//  //from wiki
//  ConsistentList<int32_t> list = { 1, 2, 3, 4, 5 };
//  auto it2 = list.begin() + 2;
//  auto it1 = it2++;
//
//  std::thread th1([&] {
//    list.erase(it1);
//  });
//
//  std::thread th2([&] {
//    list.erase(it2);
//  });
//
//  th1.join();
//  th2.join();
//
//  ASSERT_TRUE(list.size() == 3);
//}
//
//TEST(DeleteTests, Erase) {
//  ConsistentList<int32_t> list = { 1, 2, 3, 4, 5 };
//  std::vector<std::thread> threads;
//
//  for (auto it = list.begin(); it != list.end(); it++) {
//    threads.push_back(std::thread([&] {
//      list.erase(it);
//      }));
//    threads[(*it) - 1].join();
//  }
//
//
//  ASSERT_TRUE(list.empty());
//}
//
//TEST(IteratorsTests, AddAndWrite1) {
//
//  std::list<int32_t> l = { 1, 2, 3, 4, 5, 6, 7 };
//  auto lit1 = l.begin();
//  auto lit2 = std::next(lit1, 4);
//
//  ConsistentList<int32_t> list = {1, 2, 3, 4, 5, 6, 7};
//  auto it1 = list.begin();
//  auto it2 = it1;
//  it2 = it2 + 4;//TODO fix iterators +
//  std::thread thr1([&] {
//    it1 = it1 + 4;
//    });
//  //thr1.join();
//  std::thread thr2([&] {
//    *it1 = 13;
//    });
//  thr1.join();
//  ASSERT_TRUE(*(list.begin()) == 1);
//  ASSERT_TRUE(*it1 = 5);
//  thr2.join();
//  ASSERT_TRUE(*(list.begin()) == 1);
//  ASSERT_TRUE(*it1 = 13);
//}
//
//TEST(IteratorsTests, AddAndWrite2) {
//
//  std::list<int32_t> l = { 1, 2, 3, 4, 5, 6, 7 };
//  auto lit1 = l.begin();
//  auto lit2 = std::next(lit1, 4);
//
//  ConsistentList<int32_t> list = { 1, 2, 3, 4, 5, 6, 7 };
//  auto it1 = list.begin();
//  auto it2 = it1;
//  it2 = it2 + 4;//TODO fix iterators +
//  std::thread thr1([&] {
//    *it1 = 13;
//    it1 = it1 + 4;
//    });
//  thr1.join();
//  std::thread thr2([&] {
//    it1 = it1 + 4;
//    });
//  thr2.join();
//  //ASSERT_TRUE(*it1 == 5);
//  //ASSERT_TRUE(it1 == list.begin());
//
//  ASSERT_TRUE(*it1 == 13);
//}
//
//TEST(IteratorsTests, AddAndWrite3) {
//
//}
//
//TEST(IteratorsTests, SubAndWrite) {
//
//}
//
//TEST(IteratorsTests, SubAndWrite2) {
//
//}
//
//TEST(ThreadSafetyTest, RaceConditionTest) {
//  ConsistentList<int32_t> list = { 1, 2 };
//
//  std::vector<std::thread> threads;
//  auto it1 = list.begin();
//  auto it2 = it1.next();
//  static int count = 0;
//  /*for (size_t i = 0; i < 4; i++) {
//    threads.push_back(std::thread([&] {
//      //int count = 0;
//      while (count < 123) {
//        (*it1) = (*it1) + 3;
//        (*it2) = (*it2) + 2;
//        if ((*it1) % (*it2) == 0) {
//          count++;
//        }
//        std::cout << "count: " << count << *it1 << " " << *it2<< std::endl;
//      }
//      }));
//  }*/
//
//  //for (size_t i = 0; i < 4; i++) {
//  //  threads[i].join();
//  //}
//  //ASSERT_TRUE(count == 3);
//}
//
//TEST(ReadTests, HardDataOnlyReading) {
//  ConsistentList<int32_t> list;
//  for (int32_t i = 0; i < 10010; i++) {
//    list.push_back(i);
//  }
//
//  Iterator<int32_t> it = list.begin() + 124;
//
//  //int32_t for_init = 0;
//  //for (size_t i = 0; i < 8; i++) {
//  //  for (size_t j = 0; j < 125; j++) {
//  //    for_init = *(it + (j + i * 125));
//  //    std::cout << "For Obama: " << i << " " << for_init << std::endl;
//  //  }
//  //}
//
//  std::vector<std::thread> threads;
//  int32_t for_init = 0;
//
//  auto start_for_some_thread = std::chrono::high_resolution_clock::now();
//
//  for (size_t i = 0; i < 16; i++) {
//    std::cout << "AYE : " << i << std::endl;
//    threads.push_back(std::thread([&]() {
//      //for (size_t j = 0; j < 125; j++) {
//      for (auto it1 = list.begin(); it1 != list.end(); it1++) {
//        for_init = *(it1);// +(j + i * 125));
//        std::cout << "For Thread: " << std::this_thread::get_id() << " have " << for_init << std::endl;
//        //}
//      }
//      //threads[i].join();
//      }));
//  }
//  for (size_t i = 0; i < 16; i++) {
//    threads[i].join();
//  }
//
//  auto finish_for_some_thread = std::chrono::high_resolution_clock::now();
//  std::chrono::duration<double> time_of_working_some_thread =
//    finish_for_some_thread - start_for_some_thread;
//
//  it = list.begin() + 124;
//
//  std::chrono::time_point start_for_single_thread = std::chrono::high_resolution_clock::now();
//
//  for (size_t i = 0; i < 16; i++) {
//    std::cout << "AYEohhk : " << *it << i << std::endl;
//    for (auto it1 = list.begin(); it1 != list.end(); it1++) {
//      for_init = *(it1);// +(j + i * 125));
//      std::cout << "For single thread: " << i << " " << for_init << std::endl;
//    }
//    //threads[i].join();
//  }
//  std::chrono::time_point finish_for_single_thread = std::chrono::high_resolution_clock::now();
//  std::chrono::duration<double> time_of_working_single_thread =
//    finish_for_single_thread - start_for_single_thread;
//
//
//  ASSERT_TRUE(time_of_working_some_thread > time_of_working_single_thread);
//}
//
//TEST(ReadTests, HardData) {
//  ConsistentList<int32_t> list;
//  for (int32_t i = 0; i < 10010; i++) {
//    list.push_back(i);
//  }
//
//  Iterator<int32_t> it = list.begin() + 124;
//
//  //int32_t for_init = 0;
//  //for (size_t i = 0; i < 8; i++) {
//  //  for (size_t j = 0; j < 125; j++) {
//  //    for_init = *(it + (j + i * 125));
//  //    std::cout << "For Obama: " << i << " " << for_init << std::endl;
//  //  }
//  //}
//
//  std::vector<std::thread> threads;
//  int32_t for_init = 0;
//
//  auto start_for_some_thread = std::chrono::high_resolution_clock::now();
//
//  for (size_t i = 0; i < 4; i++) {
//    std::cout << "AYE : " << i << std::endl;
//    threads.push_back(std::thread([&]() {
//      //for (size_t j = 0; j < 125; j++) {
//      for (auto it1 = list.begin(); it1 != (it + ((*it) * i - 1)); it1++) {
//        for_init = *(it1);// +(j + i * 125));
//        std::cout << "For Thread: " << std::this_thread::get_id() << " have " << for_init << std::endl;
//        //}
//      }
//      //threads[i].join();
//      }));
//  }
//  for (size_t i = 0; i < 4; i++) {
//    threads[i].join();
//  }
//
//  auto finish_for_some_thread = std::chrono::high_resolution_clock::now();
//  std::chrono::duration<double> time_of_working_some_thread =
//                            finish_for_some_thread - start_for_some_thread;
//
//  it = list.begin() + 124;
//
//  std::chrono::time_point start_for_single_thread = std::chrono::high_resolution_clock::now();
//
//  for (size_t i = 0; i < 4; i++) {
//    std::cout << "AYEohhk : " << *it << i << std::endl;
//    for (auto it1 = list.begin(); it1 != (it + ((*it) * i)); it1++) {
//      for_init = *(it1);// +(j + i * 125));
//      std::cout << "For single thread: " << i << " " << for_init << std::endl;
//    }
//    //threads[i].join();
//  }
//  std::chrono::time_point finish_for_single_thread = std::chrono::high_resolution_clock::now();
//  std::chrono::duration<double> time_of_working_single_thread =
//    finish_for_single_thread - start_for_single_thread;
//  
//
//  ASSERT_TRUE(time_of_working_some_thread > time_of_working_single_thread);
//}