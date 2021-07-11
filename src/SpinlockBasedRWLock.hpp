#pragma once
#include <atomic>
#include <thread>

class RWLock {
  std::atomic <uint64_t> val = 0;
  uint64_t WRITE_BIT = (-1 >> 1) + 1;// самый левый битик 1 << 64
  
  void rlock() {
    uint64_t old = val;

    while (!(!(old & WRITE_BIT) &&
          val.compare_exchange_strong(old, old + 1, std::memory_order_acquire,
            std::memory_order_relaxed))) {

      old = val;
      
      std::this_thread::yield();


    }
  }

  void wlock() {
    uint64_t old = val;

    while (!(!(old & WRITE_BIT) &&
          val.compare_exchange_strong(old, old | WRITE_BIT, std::memory_order_acquire,
            std::memory_order_relaxed))) {

      old = val;

      std::this_thread::yield();
   
    
    }

    while (val != WRITE_BIT) {
      std::this_thread::yield();
    }
  }

  void unlock() {
    if (val == WRITE_BIT) {
      val = 0;
    }
    else {
      val--;
    }
  }
};