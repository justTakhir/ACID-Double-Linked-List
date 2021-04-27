#pragma once

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
  node_pointer prev_;
  node_pointer next_;
  size_type ref_count_;

public:
  node_reference operator =(node_const_reference other_node) {
    this->ref_count_ = other_node.ref_count_;
    this->prev_ = other_node.prev_;
    this->next_ = other_node.next_;
    return *this;
  }

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
      if (!this->getNext()->checkSentinel()){
        this->getNext()->subRefCount();
      }
      delete this;
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

  virtual bool checkSentinel() const = 0;

protected:
  Node(size_type ref_count = 2, node_pointer prev = nullptr, node_pointer next = nullptr)
    : ref_count_(ref_count), prev_(prev), next_(next) {}
};
