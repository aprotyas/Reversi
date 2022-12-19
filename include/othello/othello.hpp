#pragma once

#include <array>
#include <iostream>

#include "othello/board.hpp"
#include "othello/disk.hpp"
#include "othello/player.hpp"

class Othello {
 public:
  const Board& board() const;

  static constexpr const std::size_t num_players{2UL};

 private:
  Board m_board;
  std::array<Player, num_players> m_players;
};

std::ostream& operator<<(std::ostream& os, const Othello& othello);
