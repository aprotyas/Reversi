#pragma once

#include <array>
#include <cstddef>
#include <iostream>

#include "othello/disk.hpp"

class Board {
 public:
  Board();

  static constexpr const std::size_t size{8UL};
  std::array<std::array<Disk, size>, size>& data();
  const std::array<std::array<Disk, size>, size>& data() const;

  Disk& operator()(const std::size_t row, const std::size_t col);
  const Disk& operator()(const std::size_t row, const std::size_t col) const;

 private:
  std::array<std::array<Disk, size>, size> m_data;
};

std::ostream& operator<<(std::ostream& os, const Board& board);
