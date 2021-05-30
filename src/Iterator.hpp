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
  }

  Iterator& operator=(const Iterator& other) {
    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();
    return *this;
  }

  Iterator& operator++() {//pre
    
    std::unique_lock lock(this->ptr->getMutex());
      
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return *this;
  }

  Iterator operator++(int) {//postfix
    
    std::unique_lock lock(this->ptr->getMutex());

    auto tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return tmp;
  }

  Iterator& operator+(size_t step) {

    //std::unique_lock lock(this->ptr->getMutex());

    Iterator new_value(this->ptr);
    for (size_t i = 0; i < step; i++) {
      new_value++;
    }
    *this = new_value;
    return *this;
  }

  Iterator& operator--() {//pre

    std::unique_lock lock(this->ptr->getMutex());

    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return *this;
  }

  Iterator operator--(int) {//post

    std::unique_lock lock(this->ptr->getMutex());

    auto tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return tmp;
  }

  Iterator& operator-(size_t step) {

    //std::unique_lock lock(this->ptr->getMutex());

    Iterator new_value(this->ptr);
    for (size_t i = 0; i < step; i++) {
      new_value--;
    }
    *this = new_value;
    return *this;
  }

  T& operator* () const {
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

  T* operator ->() const {

    std::shared_lock lock(this - ptr->getMutex());
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
