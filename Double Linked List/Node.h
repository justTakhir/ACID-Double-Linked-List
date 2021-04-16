#pragma once
template<typename T>
class Node {
private:
  using node = Node<T>;
  using node_pointer = node*;
  using node_reference = node&;
  using node_const_reference = const node&;

  T value_;
  Node<T>* prev_;// this is first if = null
  Node<T>* next_;// this is last if = null
  size_t ref_count_;
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  Node() {};

  Node(value_type value, size_type ref_count = 2, node_pointer prev = nullptr, node_pointer next = nullptr)
    : value_(value), ref_count_(ref_count), prev_(prev), next_(next) {};

  node_reference operator =(node_const_reference other_node) {
    this->ref_count_ = other_node.ref_count_;
    this->prev_ = other_node.prev_;
    this->next_ = other_node.next_;
    this->value_ = other_node.value_;
    return *this;
  }

  value_type getValue() const {
    return this->value_;
  }

  void setValue(const size_type& new_value) {
    this->value_ = new_value;
  }

  size_type getRefCount() const {
    return this->ref_count_;
  }

  void setRefCount(const size_type& new_ref_count) {
    this->ref_count_ = new_ref_count;
    //this->checkEndRefCount();
  }

  void addRefCount() {
    this->ref_count_++;
  }

  void subRefCount() {
    this->ref_count_--;
  }

  void checkEndRefCount() {
    if (this->ref_count_ == 0) {
      delete this;
    }
  }

  node* getPrev() const {
    return this->prev_;
  }

  void setPrev(node* new_prev) {
    this->prev_ = new_prev;
  }

  node* getNext() const {
    return this->next_;
  }

  void setNext(node* new_next) {
    this->next_ = new_next;
  }

  virtual bool checkSentinel() = 0;
};
