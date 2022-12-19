#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

#include "othello/disk.hpp"

class Board {
 public:
  Board();

  static constexpr const std::size_t size{8UL};
  std::vector<std::vector<Disk>>& data();
  const std::vector<std::vector<Disk>>& data() const;

  Disk& operator()(const std::size_t row, const std::size_t col);
  const Disk& operator()(const std::size_t row, const std::size_t col) const;

 private:
  std::vector<std::vector<Disk>> m_data;
};

std::ostream& operator<<(std::ostream& os, const Board& board);
