#pragma once

#include "Node.hpp"
#include "ConstIterator.hpp"

template <class T>
class Iterator {
  friend class ConsistentList<T>;

public:
  Iterator(const Iterator<T>& other) : ptr(other.ptr) {
    this->ptr->addRefCount();
  }

  ~Iterator() noexcept {
    this->ptr->subRefCount();
    this->ptr->checkEndRefCount();
  }

  Iterator& operator=(const Iterator& other) {
    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();
    return *this;
  }

  Iterator& operator++() {//pre
    //do smth
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    this->ptr->getPrev()->subRefCount();
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
  }

  Iterator& operator++(int) {//post
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    this->ptr->getPrev()->subRefCount();
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator--
  Iterator& operator--() {//pre
    //do smth
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    this->ptr->getNext()->subRefCount();
    this->ptr->getNext()->checkEndRefCount();

    return *this;
  }

  Iterator& operator--(int) {//post
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    this->ptr->getNext()->subRefCount();
    this->ptr->getNext()->checkEndRefCount();

    return *this;
    //do smth
  }

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

  Iterator prev() const {
    return { this->ptr->getPrev() };
  }

  Iterator next() const {
    return { this->ptr->getNext() };
  }

private:
  Iterator(Node<T>* ptr_) noexcept : ptr(ptr_) {
    this->ptr->addRefCount();
  }

  Node<T>* ptr;
};
