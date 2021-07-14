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

    big_gc_lock.rlock();

    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    big_gc_lock.unlock();
    return *this;
  }

  ConstIterator operator++(int) {//post

    big_gc_lock.rlock();

    ConstIterator tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    big_gc_lock.unlock();
    return tmp;
  }

  ConstIterator operator+(size_t step) {

    big_gc_lock.rlock();

    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getNext();
      new_value.ptr->addRefCount();
    }

    big_gc_lock.unlock();
    return new_value;
  }

  ConstIterator operator+(int step) {

   big_gc_lock.rlock();
   
   auto new_value = *this;
   for (size_t i = 0; i < step; i++) {
     new_value.ptr->subRefCount();
     new_value.ptr = new_value.ptr->getNext();
     new_value.ptr->addRefCount();
   }

   big_gc_lock.unlock();
   return new_value;
  }

  ConstIterator& operator--() {//pre
    
    big_gc_lock.rlock();

    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    big_gc_lock.unlock();
    return *this;
  }

  ConstIterator operator--(int) {//post

    big_gc_lock.rlock();

    ConstIterator tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    big_gc_lock.unlock();
    return tmp;
  }

  ConstIterator operator-(size_t step) {

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

  ConstIterator operator-(int step) {

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

  const T& operator* () const {
    if (this->ptr->checkSentinel()) {
      throw IteratorDereferencingException("Try to dereferencing end iterator.");
    }
    this->ptr->getRWLock()->rlock();

    TrueNode<T>* ptr = dynamic_cast<TrueNode<T>*>(this->ptr);
    if (ptr == nullptr) {
      std::abort();
    }

    this->ptr->getRWLock()->unlock();
    return ptr->getValue();
  }

  const T* operator ->() const {
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

private:
  ConstIterator(const Node<T>* ptr_) : ptr(const_cast<Node<T>*>(ptr_)) {
    this->ptr->addRefCount();
  }
  Node<T>* ptr;
};
