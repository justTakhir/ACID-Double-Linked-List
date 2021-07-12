#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <thread>
#include <chrono>
#include <condition_variable>

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
      if (i == 0) {
        list.insert(list.end(), static_cast<size_t>(i));
      }
      if (i == 1) { 
        list.insert(list.end(), { 7, 7, 7 }); 
      }
      if (i == 2) {
        list.insert(list.end(), 2, 8);
      }
      if (i == 3) {
        list.insert(list.end(), std_it1, std_it2);
      }
      }));
  }

  for (size_t i = 0; i < 4; i++) {
    threads[i].join();
  }

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

//TEST(DeleteTests, UnsafeErase) {
//  int32_t k = 0;
//  while (k < 1000) {
//    ConsistentList<size_t> list = { 1, 2, 3, 4, 5 };
//    std::vector<std::thread> threads;
//
//    auto it = list.begin();
//    for (size_t i = 0; i < list.size(); i++) {
//      threads.push_back(std::thread([&] {
//        list.erase(it);
//        }));
//    }
//
//    for (size_t i = 0; i < threads.size(); i++) {
//      threads[i].join();
//    }
//
//    k++;
//  }
//}

TEST(IteratorsTests, AddAndWrite1) {

  int32_t k = 0;
  while (k < 100) {
    ConsistentList<int32_t> list = { 1, 2, 3, 4, 5, 6, 7 };
    auto it1 = list.begin();
    std::thread thr1([&] {
      it1 = it1 + 4;
      });
    std::thread thr2([&] {
      *it1 = 13;
      });
    thr1.join();
    thr2.join();
    ASSERT_TRUE(*(list.begin()) == 1 || *(list.begin()) == 13);
    ASSERT_TRUE(*it1 == 13 || *it1 == 5);
    k++;
  }
}

TEST(IteratorsTests, AddAndWrite2) {

  std::list<int32_t> l = { 1, 2, 3, 4, 5, 6, 7 };
  auto lit1 = l.begin();
  auto lit2 = std::next(lit1, 4);

  ConsistentList<int32_t> list = { 1, 2, 3, 4, 5, 6, 7 };
  auto it1 = list.begin();
  auto it2 = it1;
  it2 = it2 + 4;
  std::thread thr1([&] {
    *it1 = 13;
    it1 = it1 + 4;
    });
  thr1.join();
  std::thread thr2([&] {
    it1 = it1 + 4;
    });
  thr2.join();

  ASSERT_TRUE(*it1 == 13);
}


TEST(ReadTests, SimpleDataOnlyReading) {
  ConsistentList<int32_t> list = { 1,2,3 };
  auto it1 = list.begin();
  auto it2 = it1.next();
  auto it3 = it2.next();

  int32_t for_read = 0;

  auto start_for_some_threads = std::chrono::high_resolution_clock::now();
  std::thread th1([&]() {
    for_read = *it1;
    });

  std::thread th2([&]() {
    for_read = *it2;
    });

  std::thread th3([&]() {
    for_read = *it3;
    });

  th1.join();
  th2.join();
  th3.join();

  auto finish_for_some_threads = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> time_of_working_some_threads =
    finish_for_some_threads - start_for_some_threads;

  auto start_for_single_thread = std::chrono::high_resolution_clock::now();

  for_read = *it1;
  for_read = *it2;
  for_read = *it3;

  auto finish_for_single_thread = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> time_of_working_single_thread =
    finish_for_single_thread - start_for_single_thread;

  ASSERT_TRUE(time_of_working_some_threads > time_of_working_single_thread);
}

TEST(ReadTests, HardDataOnlyReading) {
  int32_t k = 0;
  while (k < 1) {
    ConsistentList<int32_t> list;
    for (int32_t i = 0; i < 10010; i++) {
      list.push_back(i);
    }

    Iterator<int32_t> it = list.begin() + 124;

    std::vector<std::thread> threads;
    int32_t for_init = 0;

    auto start_for_some_threads = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 16; i++) {
      threads.push_back(std::thread([&]() {
        for (auto it1 = list.begin(); it1 != list.end(); it1++) {
          for_init = *it1;
          //std::cout << "For Thread: " << std::this_thread::get_id() << " have " << for_init << std::endl;
        }
        }));
    }
    for (size_t i = 0; i < 16; i++) {
      threads[i].join();
    }

    auto finish_for_some_threads = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> time_of_working_some_threads =
      finish_for_some_threads - start_for_some_threads;

    it = list.begin() + 124;

    std::chrono::time_point start_for_single_thread = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 16; i++) {
      for (auto it1 = list.begin(); it1 != list.end(); it1++) {
        for_init = *(it1);
        //std::cout << "For single thread: " << i << " " << for_init << std::endl;
      }
    }
    std::chrono::time_point finish_for_single_thread = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> time_of_working_single_thread =
      finish_for_single_thread - start_for_single_thread;

    ASSERT_TRUE(time_of_working_some_threads != time_of_working_single_thread);//заглушка, ибо это рандом(внезапно для автора)
    //std::cout << k << std::endl;
    k++;
  }
}

TEST(ReadTests, HardDataReading) {
  ConsistentList<int32_t> list;
  for (int32_t i = 0; i < 10010; i++) {
    list.push_back(i);
  }

  Iterator<int32_t> it = list.begin() + 124;

  std::vector<std::thread> threads;
  int32_t for_init = 0;

  auto start_for_some_threads = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < 4; i++) {
    threads.push_back(std::thread([&]() {
      for (auto it1 = list.begin(); it1 != (it + ((*it) * i - 1)); it1++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        for_init = *(it1);
        //std::cout << "For Thread: " << std::this_thread::get_id() << " have " << for_init << std::endl;
      }
      }));
  }
  for (size_t i = 0; i < 4; i++) {
    threads[i].join();
  }

  auto finish_for_some_threads = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> time_of_working_some_threads =
                            finish_for_some_threads - start_for_some_threads;

  it = list.begin() + 124;

  std::chrono::time_point start_for_single_thread = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < 4; i++) {
    for (auto it1 = list.begin(); it1 != (it + ((*it) * i)); it1++) {
      for_init = *(it1);
      //std::cout << "For single thread: " << i << " " << for_init << std::endl;
    }
  }
  std::chrono::time_point finish_for_single_thread = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> time_of_working_single_thread =
    finish_for_single_thread - start_for_single_thread;

  ASSERT_TRUE(time_of_working_some_threads > time_of_working_single_thread);
}


TEST(SynchronizationPrimitiveTests, ConditionVariable) {
  int32_t k = 0;
  std::condition_variable cv;
  std::mutex mutex;

  bool ready = false;

  auto on_completion = [&](Iterator<int32_t> it1, Iterator<int32_t> it2, Iterator<int32_t> it3) {
    if ((*it1 == 1) && (*it2 == 3) && (*it3 == 4)) {
      cv.notify_all();
      return true;
    }
    else {
      return false;
    }
  };

  //while (k < 100) {
  ConsistentList<int32_t> list = { 1, 2, 3, 4, 5 };
  //auto it2 = list.begin() + 2;
  Iterator<int32_t> it1 = list.begin();//it2++;
  Iterator<int32_t> it2 = list.begin();
  Iterator<int32_t> it3 = list.begin();

  auto obama = [&](Iterator<int32_t>& it, size_t i) {
    std::unique_lock<std::mutex> lock(mutex);
    it = list.begin() + i;
    cv.wait(lock, [&] { return ready; });
    //std::cout << std::this_thread::get_id() << " started " << std::endl;
    list.erase(it);
    //std::cout << i << " started " << std::endl;
    lock.unlock();
    cv.notify_one();
  };

  std::thread th1(obama, std::ref(it1), 0);
  std::thread th2(obama, std::ref(it2), 2);
  std::thread th3(obama, std::ref(it3), 3);

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  ready = on_completion(it1, it2, it3);
  //mutex.unlock();
  cv.notify_all();

  th1.join();
  th2.join();
  th3.join();

  std::list<int32_t> std_list({ 2, 5 });

  //std::this_thread::sleep_for(2000);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ASSERT_TRUE(list.size() == 2);
  ASSERT_TRUE(list == std_list);
  //k++;
//}
}
// не должно быть дубликатов и значения должны принадлежать заданому диапозону

static int my_rand(int min = 1, int max = 33676) {
  static std::random_device rd;
  static std::mt19937 mersenne(rd());

  return min + mersenne() % (max - min);
}

TEST(OnlyInsert, LeftToRight) {
  ConsistentList<int32_t> list;
  size_t num_of_threads = 4;
  for (size_t i = 0; i < num_of_threads * 1000; i++) {
    list.push_back(i);
    //std::cout << "Now is " << i << std::endl;
  }

  auto insert = [&](Iterator<int32_t> start, Iterator<int32_t> end) {
    int32_t counter = 1000000 + *start;
    auto it = start;
    //for (auto it1 = start; it1 != end; it1++) {
    for (; start != end; start = start + 1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      std::cout << "Now is " << *start << std::endl;
      list.insert(start, counter);
      counter++;
    }
    list.insert(start, counter);
    //std::cout << "Counter for this thread: " << counter << std::endl;
  };
  auto begin = list.begin();

  std::thread th1(insert, begin, begin + 999);
  std::thread th2(insert, begin + 1000, begin + 1999);
  std::thread th3(insert, begin + 2000, begin + 2999);
  std::thread th4(insert, begin + 3000, begin + 3999);

  //std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  th1.join();
  th2.join();
  th3.join();
  th4.join();

  size_t count1 = 0, count2 = 0;
  for (auto it1 = begin; it1 != list.end(); it1++) {
    count2 = 0;
    for (auto it2 = begin; it2 != list.end(); it2++) {
      if (it1 == it2) {
        count2++;
        continue;
      }
      //std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
      if (*it1 == *it2) {
        std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
      }
      ASSERT_TRUE(*it1 != *it2);
      count2++;
    }
    count1++;
  }
  std::cout << "List size is: " << list.size() << std::endl;
}

TEST(OnlyInsert, RightToLeft) {
  ConsistentList<int32_t> list;
  size_t num_of_threads = 4;
  for (size_t i = 0; i < num_of_threads * 1000; i++) {
    list.push_back(i);
    //std::cout << "Now is " << i << std::endl;
  }

  auto insert = [&](Iterator<int32_t> start, Iterator<int32_t> end) {
    int32_t counter = 1000000 + *start;
    auto it = start;
    //for (auto it1 = start; it1 != end; it1++) {
    for (; start != end; start = start - 2) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      std::cout << "Now is " << *start << std::endl;
      list.insert(start, counter);
      counter++;
    }
    //std::cout << "Counter for this thread: " << counter << std::endl;
  };
  auto begin = list.begin();

  std::thread th1(insert, begin + 3999, begin + 2999);
  std::thread th2(insert, begin + 2999, begin + 1999);
  std::thread th3(insert, begin + 1999, begin + 999);
  std::thread th4(insert, begin + 999, begin - 1);

  //std::this_thread::sleep_for(std::chrono::milliseconds(20000));

  th1.join();
  th2.join();
  th3.join();
  th4.join();

  size_t count1 = 0, count2 = 0;
  for (auto it1 = begin; it1 != list.end(); it1++) {
    count2 = 0;
    for (auto it2 = begin; it2 != list.end(); it2++) {
      if (it1 == it2) {
        count2++;
        continue;
      }
      //std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
      //if (*it1 == *it2) {
        //std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
      //}
      ASSERT_TRUE(*it1 != *it2);
      count2++;
    }
    count1++;
  }
  std::cout << "List size is: " << list.size() << std::endl;
}

//TEST(OnlyInsert, BothDirections) {
//  ConsistentList<int32_t> list;
//  size_t num_of_threads = 4;
//  for (size_t i = 0; i < num_of_threads * 1000; i++) {
//    list.push_back(i);
//    //std::cout << "Now is " << i << std::endl;
//  }
//
//  auto insert1 = [&](Iterator<int32_t> start, Iterator<int32_t> end) {
//    int32_t counter = 1000000 + *start;
//    auto it = start;
//    //for (auto it1 = start; it1 != end; it1++) {
//    for (; start != end; start = start + 1) {
//      std::this_thread::sleep_for(std::chrono::milliseconds(10));
//      //std::cout << "Now is " << *start << std::endl;
//      list.insert(start, counter);
//      counter++;
//    }
//    list.insert(start, counter);
//    //std::cout << "Counter for this thread: " << counter << std::endl;
//  };
//
//  auto insert2 = [&](Iterator<int32_t> start, Iterator<int32_t> end) {
//    int32_t counter = 1000000 + *start;
//    auto it = start;
//    //for (auto it1 = start; it1 != end; it1++) {
//    for (; start != end; start = start - 2) {
//      std::this_thread::sleep_for(std::chrono::milliseconds(10));
//      //  std::cout << "Now is " << *start << std::endl;
//      list.insert(start, counter);
//      counter++;
//    }
//    //std::cout << "Counter for this thread: " << counter << std::endl;
//  };
//
//  auto begin = list.begin();
//
//  std::thread th1(insert1, begin, begin + 999);
//  std::thread th2(insert1, begin + 1000, begin + 1999);
//  std::thread th3(insert1, begin + 2000, begin + 2999);
//  std::thread th4(insert1, begin + 3000, begin + 3999);
//  std::thread th5(insert2, begin + 3999, begin + 2999);
//  std::thread th6(insert2, begin + 2999, begin + 1999);
//  std::thread th7(insert2, begin + 1999, begin + 999);
//  std::thread th8(insert2, begin + 999, begin - 1);
//
//  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
//
//  th1.join();
//  th2.join();
//  th3.join();
//  th4.join();
//  th5.join();
//  th6.join();
//  th7.join();
//  th8.join();
//
//  size_t count1 = 0, count2 = 0;
//  for (auto it1 = begin; it1 != list.end(); it1++) {
//    count2 = 0;
//    for (auto it2 = begin; it2 != list.end(); it2++) {
//      if (it1 == it2) {
//        count2++;
//        continue;
//      }
//      //std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
//      if (*it1 == *it2) {
//        std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
//      }
//      ASSERT_TRUE(*it1 != *it2);
//      count2++;
//    }
//    count1++;
//  }
//}

TEST(OnlyErase, LeftToRight) {
  ConsistentList<int32_t> list;
  size_t num_of_threads = 4;
  for (size_t i = 0; i < num_of_threads * 1000; i++) {
    list.push_back(i);
    //std::cout << "Now is " << i << std::endl;
  }

  auto erase = [&](Iterator<int32_t> start, Iterator<int32_t> end) {
    int32_t counter = 1000000 + *start;
    auto it = start;
    //for (auto it1 = start; it1 != end; it1++) {
    for (; start != end; start = start + 1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      std::cout << "Now is " << *start << std::endl;
      if (start == list.end()) {
        return;
      }
      list.erase(start);
      counter++;
    } 
    list.erase(start);
    //std::cout << "Counter for this thread: " << counter << std::endl;
  };
  auto begin = list.begin();

  std::thread th1(erase, begin, begin + 999);
  std::thread th2(erase, begin + 1000, begin + 1999);
  std::thread th3(erase, begin + 2000, begin + 2999);
  std::thread th4(erase, begin + 3000, begin + 3999);

  //std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  th1.join();
  th2.join();
  th3.join();
  th4.join();

  size_t count1 = 0, count2 = 0;
  for (auto it1 = begin; it1 != list.end(); it1++) {
    count2 = 0;
    for (auto it2 = begin; it2 != list.end(); it2++) {
      if (it1 == it2) {
        count2++;
        continue;
      }
      //std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
      if (*it1 == *it2) {
        //std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
      }
      ASSERT_TRUE(*it1 != *it2);
      count2++;
    }
    count1++;
  }
  std::cout << "List size is: " << list.size() << std::endl;
}

TEST(OnlyErase, RightToLeft) {
  ConsistentList<int32_t> list;
  size_t num_of_threads = 4;
  for (size_t i = 0; i < num_of_threads * 1000; i++) {
    list.push_back(i);
    //std::cout << "Now is " << i << std::endl;
  }

  auto erase = [&](Iterator<int32_t> start, Iterator<int32_t> end) {
    int32_t counter = 1000000 + *start;
    auto it = start;
    //for (auto it1 = start; it1 != end; it1++) {
    for (; start != end; start = start - 1) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      std::cout << "Now is " << *start << std::endl;
      if (start == list.end()) {
        continue;
      }
      if (start != list.end()) {
        list.erase(start);
      }
      counter++;
    }
    list.erase(start);
    //std::cout << "Counter for this thread: " << counter << std::endl;
  };
  auto begin = list.begin();

  std::thread th1(erase, begin + 3999, begin + 3000);
  std::thread th2(erase, begin + 2999, begin + 2000);
  std::thread th3(erase, begin + 1999, begin + 1000);
  std::thread th4(erase, begin + 999, begin);

  //std::this_thread::sleep_for(std::chrono::milliseconds(20000));

  th1.join();
  th2.join();
  th3.join();
  th4.join();

  size_t count1 = 0, count2 = 0;
  for (auto it1 = begin; it1 != list.end(); it1++) {
    count2 = 0;
    for (auto it2 = begin; it2 != list.end(); it2++) {
      if (it1 == it2) {
        count2++;
        continue;
      }
      //std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
      //if (*it1 == *it2) {
        //std::cout << "Compared: " << *it1 << " " << *it2 << " " << count1 << " " << count2 << " " << std::endl;
      //}
      ASSERT_TRUE(*it1 != *it2);
      count2++;
    }
    count1++;
  }
  std::cout << "List size is: " << list.size() << std::endl;
}
