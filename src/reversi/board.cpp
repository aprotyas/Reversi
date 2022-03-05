#include "board.hpp"

#include "utils.hpp"

Board::Board()
    : m_data{std::vector<std::vector<Disk>>(
          Board::m_size, std::vector<Disk>(Board::m_size, Disk::Empty))} {
  // initialize
  const std::size_t num_players{2};
  const auto start{Board::m_size / 2 - 1};
  std::size_t counter{};
  for (std::size_t row_idx{start}; row_idx < start + num_players; ++row_idx) {
    for (std::size_t col_idx{start}; col_idx < start + num_players; ++col_idx) {
      Disk output;
      if (counter++ % num_players == 0) {
        output = Disk::White;
      } else {
        output = Disk::Black;
      }
      m_data[row_idx][col_idx] = output;
    }
    ++counter;  // To flip ordering in the next row
  }
}

std::size_t Board::size() const { return Board::m_size; }

std::vector<std::vector<Disk>>& Board::data() { return m_data; }
const std::vector<std::vector<Disk>>& Board::data() const { return m_data; }

Disk& Board::operator()(const std::size_t row, const std::size_t col) {
  return m_data.at(row).at(col);
}
const Disk& Board::operator()(const std::size_t row,
                              const std::size_t col) const {
  return Board::operator()(row, col);
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
  os << "Board shape: " << board.size() << " x " << board.size() << '\n';
  for (const auto& row : board.data()) {
    os << row << '\n';
  }
  return os;
}
