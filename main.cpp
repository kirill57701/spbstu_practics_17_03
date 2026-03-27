#include <iostream>
#include <iomanip>
#include "vec.hpp"

bool testEmptVector(){
  topit::vec<int> v;
  return v.isEmpty();
}

bool testElementAccess() {
  topit::vec<int> v;
  v.pushBack(1);
  try {
    int& val = v.at(0);
    return val == 1;
  } catch (...) {
    return 0;
  }
}

bool testElementOutOfBoundAccess() {
  topit::vec<int> v;
  try {
    int & val = v.at(0);
    return 0;
  } catch (const std::out_of_range &) {
    return 1;
  } catch (...) {
    return 0;
  }
}

bool testElementInOfBoundAccess() {
  topit::vec<int> v;
  v.pushBack(1);
  topit::vec<int> rv = v;
  try {
    const int & val = v.at(0);
    return val == 1;
  } catch (...) {
    return 0;
  }
}


bool testElementInboundConstAccess() {
  topit::vec<int> v;
  v.pushBack(1);
  const topit::vec<int>& rv = v;
  try {
    const int& val = rv.at(0);
    return val == 1;
  } catch (...) {
    return false;
  }
}


bool testElementOutOfBoundConstAccess() {
  topit::vec<int> v;
  const topit::vec<int>& rv = v;
  try {
    rv.at(0);
    return false;
  } catch (const std::out_of_range &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testCopyConstructor() {
  topit::vec<int> v;
  topit::vec<int> yav{v};
  return v == yav;
}

bool testCopyConstructorForEmpty() {
  topit::vec<int> v;
  topit::vec<int> yav = v;
  return v == yav;
}

bool testCopyConstructorForNonEmpty() {
  topit::vec<int> v;
  v.pushBack(1);
  topit::vec<int> yav = v;
  try {
    return yav.getSize() == v.getSize() && yav.at(0) == v.at(0);
  } catch (...) {
    return 0;
  }
}

int main() {
  using test_t = std::pair<const char *, bool(*)() >;

  test_t tests[] = {
    {"Empty vector", testEmptVector},
    {"Element access", testElementAccess},
    {"Inbound access", testElementInOfBoundAccess},
    {"Out Of bound access", testElementOutOfBoundAccess},
    {"Inbound const access", testElementInboundConstAccess},
    {"Out of bound const access", testElementOutOfBoundConstAccess},
    {"Copy empty vector", testCopyConstructorForEmpty},
    {"Copy non-empty vector", testCopyConstructorForNonEmpty},
  };

  const size_t count = sizeof(tests)/sizeof(test_t);
  std::cout << std::boolalpha;
  bool pass = 1;

  for (size_t i = 0; i < count; ++i) {
    bool res = tests[i].second();
    std::cout << tests[i].first << ": " << res << '\n';
    pass = pass && res;
  }
  std::cout << "Result: " << pass << "\n";

  return 0;
}
