#pragma once

#include "Node.hpp"

template<typename T>
class SentinelNode : public Node<T> {
private:
public:
  SentinelNode() : Node<T>(T(), 2, this, this) {}

  bool checkSentinel() override {
    return true;
  }
};