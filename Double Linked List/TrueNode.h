#pragma once
#include "Node.h"

template<typename T>
class TrueNode : public Node<T> {
private:
public:
  TrueNode() : Node<T>() {}
  TrueNode(T value, size_t ref_count = 2, Node<T>* prev = nullptr, Node<T>* next = nullptr) :
    Node<T>(value, ref_count = 2, prev = nullptr, next = nullptr) {};

  bool checkSentinel() override {
    return false;
  }
};