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
  ConstIterator(const ConstIterator& other) : ptr(other.ptr) {
    this->ptr->addRefCount();
  }

  ~ConstIterator() {
    this->ptr->subRefCount();
  }

  ConstIterator& operator=(const ConstIterator& other) {
    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();
    return *this;
  }

  ConstIterator& operator++() {//pre

    std::unique_lock lock(this->ptr->getMutex());

    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return *this;
  }

  ConstIterator operator++(int) {//post

    std::unique_lock lock(this->ptr->getMutex());

    ConstIterator tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return tmp;
  }

  ConstIterator& operator+(size_t step) {

    //std::unique_lock lock(this->ptr->getMutex());

    ConstIterator new_value(this->ptr);
    for (size_t i = 0; i < step; i++) {
      new_value++;
    }
    *this = new_value;
    return *this;
  }

  ConstIterator& operator--() {//pre
    
    std::unique_lock lock(this->ptr->getMutex());

    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return *this;
  }

  ConstIterator operator--(int) {//post

    std::unique_lock lock(this->ptr->getMutex());

    ConstIterator tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return tmp;
  }

  ConstIterator& operator-(size_t step) {

    //std::unique_lock lock(this->ptr->getMutex());

    ConstIterator new_value(this->ptr);
    for (size_t i = 0; i < step; i++) {
      new_value--;
    }
    *this = new_value;
    return *this;
  }

  const T& operator* () const {
    if (this->ptr->checkSentinel()) {
      throw IteratorDereferencingException("Try to dereferencing end iterator.");
    }
    TrueNode<T>* ptr = dynamic_cast<TrueNode<T>*>(this->ptr);
    if (ptr == nullptr) {
      std::abort();
    }

    std::shared_lock lock(this->ptr->getMutex());
    return ptr->getValue();
  }

  const T* operator ->() const {

    std::shared_lock lock(this->ptr->getMutex());
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
