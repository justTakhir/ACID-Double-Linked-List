#pragma once
#include <shared_mutex>
#include <atomic>
#include "SpinlockBasedRWLock.hpp"
#include "GarbageCollector.hpp"

template<typename T>
class Node {
protected:
  using node = Node<T>;
  using node_pointer = node*;
  using node_reference = node&;
  using node_const_reference = const node&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

private:
  volatile node_pointer prev_;
  volatile node_pointer next_;
  volatile std::atomic<size_type> ref_count_;
  //size_type ref_count_;
  volatile std::atomic<bool> deleted_;
  RWLock rwlock_;
  volatile std::atomic<bool> died_;
  //void push_to_GC(node_type* node) {
  //  GC.nodes_.push_back()
  //}

public:
  node_reference operator =(node_const_reference other_node) {

    this->ref_count_ = other_node.ref_count_;
    this->prev_ = other_node.prev_;
    this->next_ = other_node.next_;
    return *this;
  }

  RWLock *getRWLock() {
  //RWLock getRWLock(){
    return &this->rwlock_;
  }
  
  /*void setRWLock(std::shared_mutex mut) {
    this->rwlock_ = mut;
  }*/

  size_type getRefCount() const {
    return this->ref_count_;
  }

  void setRefCount(const size_type& new_ref_count) {
    this->ref_count_ = new_ref_count;
    this->checkEndRefCount();
  }

  void addRefCount() {
    this->ref_count_++;
  }

  void subRefCount() {
    this->ref_count_--;
    this->checkEndRefCount();
  }

  void checkEndRefCount() {
    if (this->getRefCount() == 0) {
      if (!this->getPrev()->checkSentinel()) {
        this->getPrev()->subRefCount();
      }
      if (!this->getNext()->checkSentinel()) {
        this->getNext()->subRefCount();
      }
      if (!this->checkSentinel()) {
        this->died_ = true;
        //node_pointer deleted_node = this;
        //GarbageCollector<T>::takeNode(this);
      }
    }
  }

  node_pointer getPrev() {
    return this->prev_;
  }

  const node_pointer getPrev() const {
    return this->prev_;
  }

  void setPrev(node* new_prev) {
    this->prev_ = new_prev;
  }

  node_pointer getNext()  {
    return this->next_;
  }

  const node_pointer getNext() const {
    return this->next_;
  }

  void setNext(node* new_next) {
    this->next_ = new_next;
  }

  bool isDeleted() {
    return this->deleted_;
  }

  void setDeleted() {
    this->deleted_ = true;
  }

  bool isDied() {
    return this->died_;
  }
  virtual bool checkSentinel() const = 0;

protected:
  Node(size_type ref_count = 2, node_pointer prev = nullptr, node_pointer next = nullptr,
                                                    bool deleted = false, bool died = false)
    : ref_count_(ref_count), prev_(prev), next_(next), deleted_(deleted), died_(died) {}
};
