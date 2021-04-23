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
    this->ptr->checkEndRefCount();
  }

  ConstIterator& operator=(const ConstIterator& other) {
    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();
    return *this;
  }

  ConstIterator& operator++() {//pre
    //do smth
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    this->ptr->getPrev()->subRefCount();
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
  }

  ConstIterator& operator++(int) {//post
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    this->ptr->getPrev()->subRefCount();
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator--
  ConstIterator& operator--() {//pre
    //do smth
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    this->ptr->getNext()->subRefCount();
    this->ptr->getNext()->checkEndRefCount();

    return *this;
  }

  ConstIterator& operator--(int) {//post
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    this->ptr->getNext()->subRefCount();
    this->ptr->getNext()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator*
  T& operator* () const {
    if (this->ptr->checkSentinel()) {
      throw IteratorDereferencingException("Try to dereferencing end iterator.");
    }
    TrueNode<T>* ptr = dynamic_cast<TrueNode<T>*>(this->ptr);
    if (ptr == nullptr) {
      std::abort();
    }
    return ptr->getValue();
  }

  // operator->
  T* operator ->() const {
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
  ConstIterator(Node<T>* ptr_) : ptr(ptr_) {
    this->ptr->addRefCount();
  }
  Node<T>* ptr;
};
