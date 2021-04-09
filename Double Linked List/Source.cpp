#include <typeinfo>
#include <iostream>
#include <cstdint>
#include <time.h>
#include "Double_Linked_List.h"

template <typename T>
void checkType(const T& var) {
  const std::type_info& t_info = typeid(var);
  std::cout << t_info.name() << std::endl;
}

int main() {
  TrueNode<int> test_node;
  SentinelingNode<int> test_sent;
  
  int64_t b = 32;
  int64_t* a = &b;
  //checkType(test_node.getValue());
  //checkType(test_sent.getValue());
  if (test_node.checkSentinel()) {
    std::cout << "fail" << std::endl;
  }
  else {
    std::cout << "All right" << std::endl;
  }
  if (test_sent.checkSentinel()) {
    std::cout << "All right: this is really sentinel!" << std::endl;
  }
  else {
    std::cout << "fail(" << std::endl;
  }
  std::cout << "--------------NormTests---------------" << std::endl;
  Double_Linked_List<int64_t> my_list;
  TrueNode<int64_t> my_node1(b);
  my_list.insertNode(my_node1);
  return 0;
}