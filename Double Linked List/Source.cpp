#include <typeinfo>
#include <iostream>
#include <cstdint>
#include <time.h>
#include <list>
#include <algorithm>
#include <stdexcept>
//#include "Double_Linked_List.h"

int main() {
  std::list<int32_t> a;
  a.assign(3, 4);
  a.assign(2, 3);
  a.clear();

  a.assign(5, 6);
  a.resize(5);
  a.resize(5, 8);
  auto b = std::find(a.begin(), a.end(), 8);
  //a.erase(a.find(3));
  if (b != a.end()) {
    a.erase(b);
  }

  a.push_back(7);
  
  std::list<int32_t> c;
  c.assign(3, 11);
  c.merge(a);
  std::cerr << "Merged list must be ordered.";
  EXIT_FAILURE;
  
  return 0;
}