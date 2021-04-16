#pragma once
#include "Node.h"

template<typename T>
class SentinelingNode : public Node<T> {
private:
public:
  SentinelingNode() :
    Node<T>(T{}, 2, this, this) {};
  bool checkSentinel() override {
    return true;
  }
};