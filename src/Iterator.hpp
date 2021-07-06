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

    std::unique_lock<std::shared_mutex> lock(this->it_mutex);

    this->ptr->subRefCount();
    this->ptr = other.ptr;
    this->ptr->addRefCount();
    return *this;
  }

  Iterator& operator++() {//pre
    
    std::unique_lock<std::shared_mutex> lock(this->it_mutex);
      
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return *this;
  }

  Iterator operator++(int) {//postfix
    
    std::unique_lock<std::shared_mutex> lock(this->it_mutex);

    auto tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getNext();
    this->ptr->addRefCount();

    return tmp;
  }

  Iterator operator+(size_t step) {

    std::unique_lock<std::shared_mutex> lock(this->it_mutex);

    //Iterator new_value(this->ptr);
    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getNext();
      new_value.ptr->addRefCount();
    }
    //*this = new_value;
    return new_value;
  }

  Iterator operator+(int step) {

    std::unique_lock<std::shared_mutex> lock(this->it_mutex);

    //Iterator new_value(this->ptr);
    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getNext();
      new_value.ptr->addRefCount();
    }
    //*this = new_value;
    return new_value;
  }

  Iterator& operator--() {//pre

    std::unique_lock<std::shared_mutex> lock(this->it_mutex);

    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return *this;
  }

  Iterator operator--(int) {//post

    std::unique_lock<std::shared_mutex> lock(this->it_mutex);

    auto tmp = *this;
    this->ptr->subRefCount();
    this->ptr = this->ptr->getPrev();
    this->ptr->addRefCount();

    return tmp;
  }

  Iterator operator-(size_t step) {

    std::unique_lock<std::shared_mutex> lock(this->it_mutex);

    //Iterator new_value(this->ptr);
    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getPrev();
      new_value.ptr->addRefCount();
    }
    //*this = new_value;
    return new_value;
  }

  Iterator operator-(int step) {

    std::unique_lock<std::shared_mutex> lock(this->it_mutex);

    //Iterator new_value(this->ptr);
    auto new_value = *this;
    for (size_t i = 0; i < step; i++) {
      new_value.ptr->subRefCount();
      new_value.ptr = new_value.ptr->getPrev();
      new_value.ptr->addRefCount();
    }
    //*this = new_value;
    return new_value;
  }

  T& operator* () const {
    if (this->ptr->checkSentinel()) {
      throw IteratorDereferencingException("Try to dereferencing end iterator.");
    }
    
    std::shared_lock<std::shared_mutex> lock(this->it_mutex);

    TrueNode<T>* ptr = dynamic_cast<TrueNode<T>*>(this->ptr);
    if (ptr == nullptr) {
      std::abort();
    }

    //std::shared_lock lock(this->ptr->getMutex());
    return ptr->getValue();
  }

  T* operator ->() const {

    std::shared_lock<std::shared_mutex> lock(this->it_mutex);
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
  mutable std::shared_mutex it_mutex;
};
