#pragma once

#include <iostream>
#include <vector>

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
  for (const T& item : vector) {
    os << item << ", ";
  }
  return os;
}
