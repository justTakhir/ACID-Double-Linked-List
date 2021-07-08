#pragma once

#include "Node.hpp"
#include "ConstIterator.hpp"

template <class T>
class Iterator {
  friend class ConsistentList<T>;

public:
  Iterator(const Iterator<T>& other) : ptr(other.ptr), it_mutex(other.it_mutex) {
    this->ptr->addRefCount();
  }

  ~Iterator() noexcept {
    this->ptr->subRefCount();
  }

  Iterator& operator=(const Iterator& other) {

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();
    return *this;
  }

  Iterator& operator++() {//pre
    
    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));
      
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return *this;
  }

  Iterator operator++(int) {//postfix
    
    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    auto tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return tmp;
  }

  //Iterator operator+(size_t step) {

  //  std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

  //  auto new_value = *this;
  //  for (size_t i = 0; i < step; i++) {
  //    new_value.ptr->subRefCount();
  //    new_value.ptr = new_value.ptr->getNext();
  //    new_value.ptr->addRefCount();
  //  }
  //  return new_value;
  //}

  template<typename S>
  Iterator operator+(S step) {

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    auto new_value = *this;
    for (S i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getNext();
      new_value.ptr->addRefCount();
    }
    return new_value;
  }

  Iterator& operator--() {//pre

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return *this;
  }

  Iterator operator--(int) {//post

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    auto tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return tmp;
  }

  Iterator operator-(size_t step) {

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getPrev();
      new_value.ptr->addRefCount();
    }
    return new_value;
  }

  Iterator operator-(int step) {

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getPrev();
      new_value.ptr->addRefCount();
    }

    return new_value;
  }

  T& operator* () const {
    if (this->ptr->checkSentinel()) {
      throw IteratorDereferencingException("Try to dereferencing end iterator.");
    }
    
    std::shared_lock<std::shared_mutex> lock(*(this->it_mutex));

    TrueNode<T>* ptr = dynamic_cast<TrueNode<T>*>(this->ptr);
    if (ptr == nullptr) {
      std::abort();
    }

    return ptr->getValue();
  }

  T* operator ->() const {
    std::shared_lock<std::shared_mutex> lock(*(this->it_mutex));
    return &this->ptr->getValue();
  }

  bool operator!=(Iterator it) const {
    return (this->ptr != it.ptr);
  }

  bool operator==(Iterator it) const {
    return (this->ptr == it.ptr);
  }

  operator ConstIterator<T>() const {
    ConstIterator<T> obama(this->ptr);
    obama.setMutex(this->it_mutex);
    return obama;
  }

  Iterator prev() const {
    Iterator prev_it(this->ptr->getPrev());
    prev_it.setMutex(this->it_mutex);
    return prev_it;
  }

  Iterator next() const {
    Iterator next_it(this->ptr->getNext());
    next_it.setMutex(this->it_mutex);
    return next_it;
  }

  void setMutex(std::shared_mutex* mutex) {
    this->it_mutex = mutex;
  }

private:
  Iterator(Node<T>* ptr_) noexcept : ptr(ptr_) {
    this->ptr->addRefCount();
  }

  Node<T>* ptr;
  std::shared_mutex *it_mutex;
};
