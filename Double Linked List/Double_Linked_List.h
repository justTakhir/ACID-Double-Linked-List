#pragma once
#include <typeinfo>

template<typename T>
class List_Iterator {
  Node<T>* target_;

  operator --() {

  }
  
  operator ++() {
    this->target_.ref_count_--;
    if (this->target_.ref_count_ == 0) {
      
    }
    this->target_ = this->target_.next_;
  }
};

template<typename T>
class Node {
  T value_;
  Node<T>* prev_;// this is first if = null
  Node<T>* next_;// this is last if = null
  size_t ref_count_;

  Node() {};

  Node(T value, Node<T>* prev = nullptr, Node<T>* next = nullptr, size_t ref_count = 2)
                        : value_(value), prev_(prev), next_(next), ref_count_(ref_count) {}

  operator =(Node<T>* other_node) {
    this->ref_count_ = other_node->ref_count_;
    this->prev_ = other_node->prev_;
    this->next_ = other_node->next_;
    this->value_ = other_node->value_;
  }

  bool checkSentinel() const{
    return(if (typeid(this->value_) == void));
  }
};

template<typename T>
class SentinelingNode : public Node {
  //T* value_ = nullptr;//to do check for pointer
  void value_;
};

template<typename T>
class TrueNode : public Node {

};

template<typename T>
class Double_Linked_List {
  TrueNode<T>* nodes_;
  SentinelingNode<T>* head_;
  List_Iterator<T> iterator_;
  size_t size;

  Double_Linked_List() {

  }

  Node<T>* searchNode(const &T value) {
    TrueNode<T> searched_node = this->head_;
    while (searched_node != nullptr && searched_node.value_ != value) {
      searched_node = searched_node.next_;
    }
    return searched_node;
  }

  Node<T>* insertNode(Node<T> inserted_node) {
    inserted_node.next_ = this->head_;
    if (this->head_ != nullptr) {
      this->head_->prev_ = inserted_node;
    }
    this->head_ = inserted_node;
    inserted_node.prev_ = nullptr;
    this->size++;
  }

  Node<T>* insertNode(const &T value) {
    TrueNode<T> inserted_node(value);
    this->insertNode(inserted_node);
  }

  /*Node<T>**/ deleteNode(const &T value) {
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
    this->size--;
  }

};
