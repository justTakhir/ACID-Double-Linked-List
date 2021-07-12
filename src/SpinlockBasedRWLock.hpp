#pragma once
#include <atomic>
#include <thread>

class RWLock {
private:
  std::atomic <uint64_t> val;// = 0;
  uint64_t WRITE_BIT;// = (-1 >> 1) + 1;// самый левый битик 1 << 64
  
public:
  RWLock() {
    this->val.store(0);
    this->WRITE_BIT = (-1 >> 1) + 1;
  }

  //RWLock(const RWLock& other) {
  //  this->val.store(other.val.load());
  //  this->WRITE_BIT = other.WRITE_BIT;
  //}

  void rlock() {
    //uint64_t old = val;

    while (true) {

      uint64_t old = val;
      
      if (!(old & WRITE_BIT) &&
        val.compare_exchange_strong(old, old + 1, std::memory_order_acquire,
          std::memory_order_relaxed)) {
        return;
      }

      std::this_thread::yield();


    }
  }

  void wlock() {
    //uint64_t old = val;

    while (true) {
      uint64_t old = val;
      if (!(old & WRITE_BIT) && val.compare_exchange_strong(old, old | WRITE_BIT)) {
        break;
      }
      std::this_thread::yield();
    }
    while (true) {
      if (val == WRITE_BIT) {
        break;
      }
      std::this_thread::yield();
    }

    /*while (!(!(old & WRITE_BIT) &&
          val.compare_exchange_strong(old, old | WRITE_BIT,
            std::memory_order_relaxed))) {

      old = val;

      std::this_thread::yield();


    }

    while (val != WRITE_BIT) {
      std::this_thread::yield();
    }
  }*/
  }
  void unlock() {
    if (val == WRITE_BIT) {
      val = 0;
    }
    else {
      val--;
    }
  }

  //~RWLock() {
  //  this->unlock();
  //}
};