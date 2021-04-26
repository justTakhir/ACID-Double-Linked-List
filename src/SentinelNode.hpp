#pragma once

#include "Node.hpp"

template<typename T>
class SentinelNode : public Node<T> {
public:
  SentinelNode() : Node<T>(2, this, this) {}

  bool checkSentinel() const override {
    return true;
  }
};