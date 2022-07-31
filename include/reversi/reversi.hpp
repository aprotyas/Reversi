#pragma once

#include <array>
#include <iostream>

#include "reversi/board.hpp"
#include "reversi/disk.hpp"
#include "reversi/player.hpp"

class Reversi {
 public:
  Reversi() = default;
  ~Reversi() = default;
  const Board& board() const;

 private:
  Board m_board;
  std::array<Player, 2> m_players;
};

std::ostream& operator<<(std::ostream& os, const Reversi& reversi);
