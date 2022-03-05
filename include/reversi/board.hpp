#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

#include "disk.hpp"

class Board
{
public:
  Board();

  std::size_t size() const;
  std::vector<std::vector<Disk>>& data();
  const std::vector<std::vector<Disk>>& data() const;

  Disk& operator()(const std::size_t row, const std::size_t col);
  const Disk& operator()(const std::size_t row, const std::size_t col) const;

private:
  std::vector<std::vector<Disk>> m_data;
  constexpr static std::size_t m_size = 8;
};

std::ostream& operator<<(std::ostream& os, const Board& board);
