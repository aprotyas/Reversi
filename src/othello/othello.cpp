#include "othello/othello.hpp"

#include <iostream>

#include "othello/board.hpp"

std::ostream& operator<<(std::ostream& os, const Othello& othello) {
  os << "--- Othello: Round " << othello.round() << "---\n";
  os << othello.board() << '\n';
  return os;
}

const Board& Othello::board() const { return m_board; }
std::size_t Othello::round() const { return m_round; }
