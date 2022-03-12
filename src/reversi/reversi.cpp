#include <iostream>

#include "reversi/board.hpp"
#include "reversi/reversi.hpp"

std::ostream& operator<<(std::ostream& os, const Reversi& reversi) {
  os << "--- Reversi state ---\n";
  os << reversi.board() << '\n';
  return os;
}

const Board& Reversi::board() const { return m_board; }
