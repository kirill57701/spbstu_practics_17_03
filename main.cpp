#include <iostream>
#include "vec.hpp"

bool testEmptVec(){
  return 0;
}

int main() {
  using test_t = std::pair<const char *, bool(*)() >; 
  test_t tests[] = {
    {"Empty vector", testEmptVec}
  };
  const size_t count = sizeof(tests)/sizeof(test_t);
  std::cout << std::boolalpha;
  for (size_t i = 0; i < count; ++i) {
    std::cout << tests[i].first << ": " << tests[i].second() << '\n';
  }
}
