#include <iostream>
#include <iomanip>
#include <utility>
#include <stdexcept>
#include "vec.hpp"

bool testEmptyVector() {
  topit::vec< int > v;
  return v.isEmpty();
}

bool testElementInboundAccess() {
  topit::vec< int > v;
  v.pushBack(1);
  try {
    int & val = v.at(0);
    return val == 1;
  } catch (...) {
    return false;
  }
}

bool testElementOutOfBoundAccess() {
  topit::vec< int > v;
  try {
    v.at(0);
    return false;
  } catch (const std::out_of_range &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testElementInboundConstAccess() {
  topit::vec< int > v;
  v.pushBack(1);
  const topit::vec< int > & rv = v;
  try {
    const int & val = rv.at(0);
    return val == 1;
  } catch (...) {
    return false;
  }
}

bool testElementOutOfBoundConstAccess() {
  const topit::vec< int > v;
  try {
    v.at(0);
    return false;
  } catch (const std::out_of_range &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testCopyConstructorForEmpty() {
  topit::vec< int > v;
  topit::vec< int > yav = v;
  return v == yav;
}

bool testCopyConstructorForNonEmpty() {
  topit::vec< int > v;
  v.pushBack(1);
  topit::vec< int > yav = v;
  try {
    return yav.getSize() == v.getSize() && yav.at(0) == v.at(0);
  } catch (...) {
    return false;
  }
}

bool testInitializerList() {
  topit::vec< int > v{1, 2};
  return v.getSize() == 2 && (v[0] == 1) && (v[1] == 2);
}

bool testInsertPos() {
  topit::vec<int> v({7, 77, 3});
  auto it = v.begin() + 1;
  v.insert(it, 5);
  return v.getSize() == 4 && v[1] == 5 && v[2] == 77;
}

bool testInsertCount() {
  topit::vec<int> v({101, 111});
  v.insert(v.begin() + 1, static_cast<size_t>(3), 73);
  return v.getSize() == 5 && v[1] == 73 && v[4] == 111;
}

int main()
{
  using test_t = std::pair< const char*, bool(*)() >;
  test_t tests[] = {
    {"Empty vector", testEmptyVector},
    {"Inbound access", testElementInboundAccess},
    {"Out of bound access", testElementOutOfBoundAccess},
    {"Inbound const access", testElementInboundConstAccess},
    {"Out of bound const accsess", testElementOutOfBoundConstAccess},
    {"Copy empty vector", testCopyConstructorForEmpty},
    {"Copy non-empty vector", testCopyConstructorForNonEmpty},
    {"Non-empty vector for non-empty initializer list", testInitializerList},
    {"Test of insert into pos", testInsertPos},
    {"Test of insert count", testInsertCount},
  };

  const size_t count = sizeof(tests) / sizeof(test_t);
  std::cout << std::boolalpha;
  bool pass = true;

  for (size_t i = 0; i < count; ++i) {
    bool res = tests[i].second();
    std::cout << tests[i].first << ": " << res << "\n";
    pass = pass && res;
  }

  std::cout << "RESULT: " << pass << "\n";
  return 0;
}