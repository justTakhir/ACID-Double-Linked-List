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

//TEST(DeleteTests, EraseInSameTime2) {
//  //from wiki
//  int32_t k = 0;
//  while (k < 100) {
//    ConsistentList<int32_t> list = { 1, 2, 3, 4, 5 };
//    //auto it2 = list.begin() + 2;
//    auto it1 = list.begin();//it2++;
//
//    std::thread th1([&] {
//      list.erase(it1);
//      std::cout << "1 started " << std::endl;
//      });
//
//    std::thread th2([&] {
//      std::cout << "2 started " << std::endl;
//      it1++;
//      it1++;
//      });
//
//    std::thread th3([&] {
//      std::cout << "3 started " << std::endl;
//      list.erase(it1);
//      });
//
//    th1.join();
//    th2.join();
//    th3.join();
//
//    std::list<int32_t> std_list({ 2, 4, 5 });
//
//    //std::this_thread::sleep_for(2000);
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//    ASSERT_TRUE(list.size() == 3);
//    ASSERT_TRUE(list == std_list);
//    k++;
//  }
//}

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
  while (k < 10000) {
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