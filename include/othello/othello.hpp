#pragma once

#include <array>
#include <iostream>

#include "othello/board.hpp"
#include "othello/disk.hpp"
#include "othello/player.hpp"

class Othello {
 public:
  Othello() = default;
  ~Othello() = default;
  const Board& board() const;

 private:
  static constexpr const std::size_t num_players{2UL};

  Board m_board;
  std::array<Player, num_players> m_players;
};

std::ostream& operator<<(std::ostream& os, const Othello& othello);
