#pragma once
#include "Node.h"
#include "ConstIterator.h"

template <class T>
class Iterator {
  friend class ConsistentList<T>;
public:

  Iterator& operator++() {//pre
    //do smth
    this->ptr = this->ptr->getNext();

    this->ptr->getPrev()->setRefCount(this->ptr->getPrev()->getRefCount() - 1);
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
  }

  Iterator& operator++(int) {//post
    this->ptr = this->ptr->getNext();

    this->ptr->getPrev()->setRefCount(this->ptr->getPrev()->getRefCount - 1);
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator--
  Iterator& operator--() {//pre
    //do smth
    this->ptr = this->ptr->getPrev();

    this->ptr->getNext()->setRefCount(this->ptr->getNext()->getRefCount() - 1);
    this->ptr->getNext()->checkEndRefCount();

    return *this;
  }

  Iterator& operator--(int) {//post
    this->ptr = this->ptr->getPrev();

    this->ptr->getNext()->setRefCount(this->ptr->getNext()->getRefCount() - 1);
    this->ptr->getNext()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator*
  T& operator* () const {
    return this->ptr->getValue();
  }

  // operator->
  T* operator ->() const {
    return &this->ptr->getValue();
  }

  bool operator!=(Iterator it) const {
    return (this->ptr != it.ptr);
  }

  bool operator==(Iterator it) const {
    return (this->ptr == it.ptr);
  }

  operator ConstIterator<T>() const {
    return ConstIterator<T>(this->ptr);
  }

  ~Iterator() {
    this->ptr->setRefCount(this->ptr->getRefCount() - 1);
    this->ptr->checkEndRefCount();
  }

private:
  Iterator(Node<T>* ptr_) : ptr(ptr_) {}
  Node<T>* ptr;
};