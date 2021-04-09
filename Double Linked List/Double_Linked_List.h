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

  /*Node<T>**/
  auto getPrev() const {
    return this->prev_;
  }
  
  void setPrev(Node<T>* new_prev) {
    this->prev_ = new_prev;
  }

  /*Node<T>**/
  auto getNext() const {
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
  using Node<T>::setNext;
  void setNext(TrueNode<T>* new_prev) {};
  using Node<T>::setPrev;
  void setPrev(TrueNode<T>* new_prev) {};
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

  /*Node<T>**/
  auto searchNode(const T &value) {
    //TrueNode<T> searched_node = this->head_;
    TrueNode<T> searched_node = this->sentinel_.getNext();
    while ((!searched_node.checkSentinel()) && searched_node.getValue() != value) {
      searched_node = searched_node.getNext();
    }
    if (searched_node.checkSentinel()) {//if random value in sentinel such as searched value
      return nullptr;
    }
    else {
      return searched_node;
    }
  }

  void insertNode(TrueNode<T> inserted_node) {
    inserted_node.setNext(this->sentinel_.getNext());
    this->sentinel_.getNext()->setPrev(inserted_node);
    this->sentinel_.setNext(inserted_node);
    inserted_node.setPrev(this->sentinel_);
    this->size_++;
  }

  void insertNode(const T &value) {
    TrueNode<T> inserted_node(value);
    this->insertNode(inserted_node);
  }

  void deleteNode(const T &value) {
    TrueNode<T> deletedNode = searchNode(value);
    if (deletedNode == nullptr) {
      std::cout << "Such node doesn't exist!" << std::endl;
      return;
    }

    //deletedNode.getPrev()->setNext(deletedNode.getNext());
    //deletedNode.getNext()->setPrev(deletedNode.getPrev());
    deletedNode.getPrev()->getNext() = deletedNode.getNext();
    deletedNode.getNext()->getPrev() = deletedNode.getPrev();
    this->size_--;
  }

};
