#pragma once

#include "Node.hpp"

template<typename T>
class TrueNode : public Node<T> {
private:
  T value_;

public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = value_type&;
  using const_reference = const value_type&;

  TrueNode(const_reference value, node_pointer prev, node_pointer next)
    : Node<T>(2, prev, next), value_(value) {}

  TrueNode& operator= (const TrueNode& other) {
    Node<T>::operator= (other);
    this->value_ = other.value_;
    return *this;
  }

  reference getValue() {
    return this->value_;
  }

  void setValue(const_reference new_value) {
    this->value_ = new_value;
  }

  bool checkSentinel() override {
    return false;
  }
};