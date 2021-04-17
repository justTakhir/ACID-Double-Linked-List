#include "ConsistentList.h"
#include <list>

int main() {
  ConsistentList<int64_t> oboba;
  Iterator<int64_t> it = oboba.insert(oboba.begin(), 2);
  it = oboba.insert(oboba.begin(), 7);
  Iterator<int64_t>it2(oboba.end().prev());
  it2 = it;
  std::cout << "" << std::endl;
  std::list<int64_t> l;
  auto i = l.begin();
  l.insert(l.begin(), 2);
  l.insert(l.begin(), 7);
  return 0;
}