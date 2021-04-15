#pragma once
#include "Node.h"

template<typename T>
class SentinelingNode : public Node<T> {
private:
public:
  SentinelingNode() : Node<T>() {}
  SentinelingNode(T value, size_t ref_count = 2, Node<T>* prev = nullptr, Node<T>* next = nullptr) :
    Node<T>(value, ref_count, prev, next) {};
  bool checkSentinel() override {
    return true;
  }
};