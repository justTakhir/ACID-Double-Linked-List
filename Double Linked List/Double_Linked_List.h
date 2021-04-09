#pragma once
#include <typeinfo>

template<typename T>
class Node {
 private:
  T value_;
  Node<T>* prev_;// this is first if = null
  Node<T>* next_;// this is last if = null
  size_t ref_count_;
 public:
  Node() {};

  Node(T value, size_t ref_count = 2, Node<T>* prev = nullptr, Node<T>* next = nullptr)
                        : value_(value), ref_count_(ref_count), prev_(prev), next_(next) {}

  Node<T> operator =(const Node<T> &other_node) {
    this->ref_count_ = other_node->ref_count_;
    this->prev_ = other_node->prev_;
    this->next_ = other_node->next_;
    this->value_ = other_node.value_;
  }

  T getValue() const {
    return this->value_;
  }

  void setValue(const size_t& new_value) {
    this->value_ = new_value;
  }

  size_t getRefCount() const {
    return this->ref_count_;
  }

  void setRefCount(const size_t &new_value) {
    this->value_ = new_value;
  }

  Node<T>* getPrev() const {
    return this->prev_;
  }

  void setPrev(Node<T>* new_prev) {
    this->prev_ = new_prev;
  }

  Node<T>* getNext() const {
    return this->next_;
  }

  void setNext(Node<T>* new_next) {
    this->next_ = new_next;
  }

  virtual bool checkSentinel() = 0;
};

template<typename T>
class SentinelingNode : public Node<T> {
 private:
 public:
  bool checkSentinel() override {
    return true;
  }
};

template<typename T>
class TrueNode : public Node<T> {
 private:
 public:
  bool checkSentinel() override {
    return false;
  }
};

template<typename T>
class List_Iterator {
  Node<T>* target_;

  List_Iterator<T> operator --() {

  }

  List_Iterator<T> operator ++() {
    this->target_.ref_count_--;
    if (this->target_.ref_count_ == 0) {

    }
    this->target_ = this->target_.next_;
  }
};

template<typename T>
class Double_Linked_List {
 private:
  //TrueNode<T>* nodes_;
  SentinelingNode<T> sentinel_;
  List_Iterator<T> iterator_;
  size_t size_ = 0;
 public:
  Double_Linked_List() {

  }

  Node<T>* searchNode(const T &value) {
    //TrueNode<T> searched_node = this->head_;
    TrueNode<T> searched_node = this->sentinel_.getNext();
    while ((!searched_node.checkSentinel()) && searched_node.getValue() != value) {
      searched_node = searched_node.getNext();
    }
    if (searched_node.checkSentinel()) {//if random value in sentinel such as 
      return nullptr;
    }
    else {
      return searched_node;
    }
  }

  Node<T>* insertNode(Node<T> inserted_node) {
    inserted_node.next_ = this->head_;
    if (this->head_ != nullptr) {
      this->head_->prev_ = inserted_node;
    }
    this->head_ = inserted_node;
    inserted_node.prev_ = nullptr;
    this->size_++;
  }

  Node<T>* insertNode(const T &value) {
    TrueNode<T> inserted_node(value);
    this->insertNode(inserted_node);
  }

  void deleteNode(const T &value) {
    TrueNode<T> deletedNode = searchNode(value);
    if (deletedNode == nullptr) {
      std::cout << "Such node doesn't exist!" << std::endl;
      return;
    }

    if (deletedNode.prev_ != nullptr) {
      deletedNode.prev_->next_ = deletedNode.next_;
    }
    else {
      this->head_ = deletedNode.next_;
    }
    if (deletedNode.next_ != nullptr) {
      deletedNode.next_->prev_ = deletedNode.prev_;
    }
    this->size_--;
  }

};
