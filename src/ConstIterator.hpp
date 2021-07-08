#pragma once

#include "Node.hpp"

template<class T>
class ConsistentList;

template<class T>
class Iterator;

template <class T>
class ConstIterator {
  friend class ConsistentList<T>;
  friend class Iterator<T>;

public:
  ConstIterator(const ConstIterator& other) : ptr(other.ptr), it_mutex(other.it_mutex) {
    this->ptr->addRefCount();
  }

  ~ConstIterator() {
    this->ptr->subRefCount();
  }

  ConstIterator& operator=(const ConstIterator& other) {

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();
    return *this;
  }

  ConstIterator& operator++() {//pre

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return *this;
  }

  ConstIterator operator++(int) {//post

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    ConstIterator tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return tmp;
  }

  ConstIterator operator+(size_t step) {

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getNext();
      new_value.ptr->addRefCount();
    }

    return new_value;
  }

  ConstIterator operator+(int step) {

   std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

   auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getNext();
      new_value.ptr->addRefCount();
    }

    return new_value;
  }

  ConstIterator& operator--() {//pre
    
    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return *this;
  }

  ConstIterator operator--(int) {//post

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    ConstIterator tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return tmp;
  }

  ConstIterator operator-(size_t step) {

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));


    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getPrev();
      new_value.ptr->addRefCount();
    }

    return new_value;
  }

  ConstIterator operator-(int step) {

    std::unique_lock<std::shared_mutex> lock(*(this->it_mutex));

    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getPrev();
      new_value.ptr->addRefCount();
    }

    return new_value;
  }

  const T& operator* () const {
    if (this->ptr->checkSentinel()) {
      throw IteratorDereferencingException("Try to dereferencing end iterator.");
    }
    TrueNode<T>* ptr = dynamic_cast<TrueNode<T>*>(this->ptr);
    if (ptr == nullptr) {
      std::abort();
    }

    std::shared_lock<std::shared_mutex> lock(*(this->it_mutex));
    return ptr->getValue();
  }

  const T* operator ->() const {

    std::shared_lock<std::shared_mutex> lock(*(this->it_mutex));
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

  void setMutex(std::shared_mutex* mutex) {
    this->it_mutex = mutex;
  }

private:
  ConstIterator(const Node<T>* ptr_) : ptr(const_cast<Node<T>*>(ptr_)) {
    this->ptr->addRefCount();
  }
  Node<T>* ptr;
  std::shared_mutex *it_mutex;
};
