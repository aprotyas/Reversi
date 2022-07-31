#include "reversi/disk.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Disk disk) {
  switch (disk) {
    case Disk::Empty:
      os << ' ';
      break;
    case Disk::White:
      os << "\u25e6";
      break;
    case Disk::Black:
      os << "\u00b7";
      break;
  }
  return os;
}
