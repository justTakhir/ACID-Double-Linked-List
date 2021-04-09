#include <typeinfo>
#include <iostream>
#include "Double_Linked_List.h"

template <typename T>
void checkType(const T& var) {
  const std::type_info& t_info = typeid(var);
  std::cout << t_info.name() << std::endl;
}

int main() {
  TrueNode<int> test_node;
  SentinelingNode<int> test_sent;
  int b = 32;
  int* a = &b;
  checkType(test_node.getValue());
  checkType(test_sent.getValue());
  return 0;
}