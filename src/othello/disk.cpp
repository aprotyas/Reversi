#include "othello/disk.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Disk disk) {
  switch (disk) {
    case Disk::Empty:
      os << '-';
      break;
    case Disk::Light:
      os << 'X';
      break;
    case Disk::Dark:
      os << '#';
      break;
  }
  return os;
}
