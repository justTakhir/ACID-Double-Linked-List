#pragma once

#include "Node.hpp"

template<typename T>
class TrueNode : public Node<T> {
private:
public:
  TrueNode() : Node<T>(T(), 2, nullptr, nullptr) {}

  bool checkSentinel() override {
    return false;
  }
  
};