#pragma once

#include "Node.hpp"
#include "GarbageCollector.hpp"

template<class T>
class ConsistentList;

template<class T>
class Iterator;

template <class T>
class ConstIterator {
  friend class ConsistentList<T>;
  friend class Iterator<T>;

public:
  ConstIterator(const ConstIterator& other) : ptr(other.ptr) {
    this->ptr->addRefCount();
  }

  ~ConstIterator() {
    this->ptr->subRefCount();
  }

  ConstIterator& operator=(const ConstIterator& other) {

    //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    other.ptr->getRWLock()->wlock();
    this->ptr->getRWLock()->wlock();

    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();

    other.ptr->getRWLock()->unlock();
    this->ptr->getRWLock()->unlock();
    return *this;
  }

  ConstIterator& operator++() {//pre

    //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->wlock();
    big_gc_lock.wlock();

    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    //this->ptr->getRWLock()->unlock();
    big_gc_lock.unlock();
    return *this;
  }

  ConstIterator operator++(int) {//post

    //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->wlock();
    big_gc_lock.wlock();

    ConstIterator tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    //this->ptr->getRWLock()->unlock();
    big_gc_lock.unlock();
    return tmp;
  }

  ConstIterator operator+(size_t step) {

    //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->wlock();
    big_gc_lock.wlock();

    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getNext();
      new_value.ptr->addRefCount();
    }

    //this->ptr->getRWLock()->unlock();
    big_gc_lock.unlock();
    return new_value;
  }

  ConstIterator operator+(int step) {

   //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
   //this->ptr->getRWLock()->wlock();
   big_gc_lock.wlock();
   
   auto new_value = *this;
   for (size_t i = 0; i < step; i++) {
     new_value.ptr->subRefCount();
     new_value.ptr = new_value.ptr->getNext();
     new_value.ptr->addRefCount();
   }

   //this->ptr->getRWLock()->unlock();
   big_gc_lock.unlock();
   return new_value;
  }

  ConstIterator& operator--() {//pre
    
    //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->wlock();
    big_gc_lock.wlock();

    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    //this->ptr->getRWLock()->unlock();
    big_gc_lock.unlock();
    return *this;
  }

  ConstIterator operator--(int) {//post

    //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->wlock();
    big_gc_lock.wlock();

    ConstIterator tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    //this->ptr->getRWLock()->unlock();
    big_gc_lock.unlock();
    return tmp;
  }

  ConstIterator operator-(size_t step) {

    //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->wlock();
    big_gc_lock.wlock();

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

  ConstIterator operator-(int step) {

    //std::unique_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->wlock();
    big_gc_lock.wlock();

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

  const T& operator* () const {
    if (this->ptr->checkSentinel()) {
      throw IteratorDereferencingException("Try to dereferencing end iterator.");
    }
    this->ptr->getRWLock()->rlock();

    TrueNode<T>* ptr = dynamic_cast<TrueNode<T>*>(this->ptr);
    if (ptr == nullptr) {
      std::abort();
    }

    //std::shared_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->rlock();
    this->ptr->getRWLock()->unlock();
    return ptr->getValue();
  }

  const T* operator ->() const {

    //std::shared_lock<std::shared_mutex> lock(this->ptr->getRWLock());
    //this->ptr->getRWLock()->rlock();
    return &this->ptr->getValue();
  }

  bool operator!=(ConstIterator it) const {
    return (this->ptr != it.ptr);
  }

  bool operator==(ConstIterator it) const {
    return (this->ptr == it.ptr);
  }

  ConstIterator prev() const {
    return { this->ptr->getPrev() };
  }

  ConstIterator next() const {
    return { this->ptr->getNext() };
  }

  /*void setMutex(std::shared_mutex* mutex) {
    this->it_mutex = mutex;
  }*/

private:
  ConstIterator(const Node<T>* ptr_) : ptr(const_cast<Node<T>*>(ptr_)) {
    this->ptr->addRefCount();
  }
  Node<T>* ptr;
  //std::shared_mutex *it_mutex;
};
