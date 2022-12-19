#include "othello/othello.hpp"

#include <iostream>

#include "othello/board.hpp"

std::ostream& operator<<(std::ostream& os, const Othello& othello) {
  os << "--- Othello state ---\n";
  os << othello.board() << '\n';
  return os;
}

const Board& Othello::board() const { return m_board; }
