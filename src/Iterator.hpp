#pragma once

#include "Node.hpp"
#include "ConstIterator.hpp"
#include "GarbageCollector.hpp"

template <class T>
class Iterator {
  friend class ConsistentList<T>;

public:
  Iterator(const Iterator<T>& other) : ptr(other.ptr) {
    this->ptr->addRefCount();
  }

  ~Iterator() noexcept {
    this->ptr->subRefCount();
  }

  Iterator& operator=(const Iterator& other) {

    other.ptr->getRWLock()->wlock();
    RWLock* lock = this->ptr->getRWLock();
    lock->wlock();

    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();

    lock->unlock();
    other.ptr->getRWLock()->unlock();
    return *this;
  }

  Iterator& operator++() {//pre

    big_gc_lock.rlock();

    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    big_gc_lock.unlock();
    return *this;
  }

  Iterator operator++(int) {//postfix
    
    big_gc_lock.rlock();

    auto tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    big_gc_lock.unlock();
    return tmp;
  }

  template<typename S>
  Iterator operator+(S step) {

    big_gc_lock.rlock();

    auto new_value = *this;
    for (S i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getNext();
      new_value.ptr->addRefCount();
    }

    big_gc_lock.unlock();
    return new_value;
  }

  Iterator& operator--() {//pre

    big_gc_lock.rlock();

    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    big_gc_lock.unlock();
    return *this;
  }

  Iterator operator--(int) {//post

    big_gc_lock.rlock();

    auto tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    big_gc_lock.unlock();
    return tmp;
  }

  Iterator operator-(size_t step) {

    big_gc_lock.rlock();

    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getPrev();
      new_value.ptr->addRefCount();
    }

    big_gc_lock.unlock();
    return new_value;
  }

  Iterator operator-(int step) {

    big_gc_lock.rlock();

    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getPrev();
      new_value.ptr->addRefCount();
    }

    //this->ptr->getRWLock()->unlock();
    big_gc_lock.unlock();
    
    return new_value;
  }

  T& operator* () const {
    if (this->ptr->checkSentinel()) {
      throw IteratorDereferencingException("Try to dereferencing end iterator.");
    }
    
    //std::shared_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    this->ptr->getRWLock()->rlock();

    TrueNode<T>* ptr = dynamic_cast<TrueNode<T>*>(this->ptr);
    if (ptr == nullptr) {
      std::abort();
    }
    this->ptr->getRWLock()->unlock();
    return ptr->getValue();
  }

  T* operator ->() const {
    //std::shared_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->rlock();

    return &this->ptr->getValue();
  }

  bool operator!=(Iterator it) const {
    return (this->ptr != it.ptr);
  }

  bool operator==(Iterator it) const {
    return (this->ptr == it.ptr);
  }

  operator ConstIterator<T>() const {
    /*ConstIterator<T> obama(this->ptr);
    obama.setMutex(this->it_mutex);*/
    return { this->ptr };
  }

  Iterator prev() const {
    //Iterator prev_it(this->ptr->getPrev());
    //prev_it.setMutex(this->it_mutex);
    return { this->ptr->getPrev() };
  }

  Iterator next() const {
    //Iterator next_it(this->ptr->getNext());
    //next_it.setMutex(this->it_mutex);
    return { this->ptr->getNext() };
  }

  /*void setMutex(std::shared_mutex* mutex) {
    this->it_mutex = mutex;
  }*/

private:
  Iterator(Node<T>* ptr_) noexcept : ptr(ptr_) {
    this->ptr->addRefCount();
  }

  Node<T>* ptr;
  //RWLock* gc_lock;// = &GarbageCollector::getLock();
  //std::shared_mutex *it_mutex;
};
