#include "othello/board.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

#include "othello/othello.hpp"

Board::Board() {
  // initialize
  std::for_each(m_data.begin(), m_data.end(),
                [](auto& row) { row.fill(Disk::Empty); });

  const auto start{Board::size / 2 - 1};
  for (std::size_t row_idx{start}, counter{};
       row_idx < start + Othello::num_players; ++row_idx, ++counter) {
    for (std::size_t col_idx{start}; col_idx < start + Othello::num_players;
         ++col_idx, ++counter) {
      Disk output;
      if (counter % Othello::num_players == 0) {
        output = Disk::Light;
      } else {
        output = Disk::Dark;
      }
      m_data[row_idx][col_idx] = output;
    }
  }
}

std::array<std::array<Disk, Board::size>, Board::size>& Board::data() {
  return m_data;
}
const std::array<std::array<Disk, Board::size>, Board::size>& Board::data()
    const {
  return m_data;
}

Disk& Board::operator()(const std::size_t row, const std::size_t col) {
  return m_data.at(row).at(col);
}
const Disk& Board::operator()(const std::size_t row,
                              const std::size_t col) const {
  return m_data.at(row).at(col);
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
  for (const auto& row : board.data()) {
    for (const auto& elem : row) {
      os << elem << ' ';
    }
    os << '\n';
  }
  return os;
}
