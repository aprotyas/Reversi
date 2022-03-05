#include <cstddef>
#include <cassert>
#include <vector>
#include <iostream>
#include <memory>
#include <tuple>

enum class Disk : char {
  Empty = ' ',
  Black = 'b',
  White = 'w'
};

class Board
{
public:
  Board()
  : m_data {std::vector<std::vector<Disk>>(m_size, std::vector<Disk>(m_size, Disk::Empty))}
  {
    // initialize
    const std::size_t num_players {2};
    const auto start {m_size/2 - 1};
    std::size_t counter{};
    for (std::size_t row_idx {start}; row_idx < start + num_players; ++row_idx) {
      for (std::size_t col_idx {start}; col_idx < start + num_players; ++col_idx) {
        Disk output;
        if (counter++ % num_players == 0) {
          output = Disk::White;
        }
        else {
          output = Disk::Black;
        }
        m_data[row_idx][col_idx] = output;
      }
      ++counter; // To flip ordering in the next row
    }
  }

  std::size_t size() const { return m_size; }
  auto& data() { return m_data; }
  const auto& data() const { return m_data; }

  Disk& operator()(std::size_t row, std::size_t col) {
    return m_data[row][col];
  }
  const Disk& operator()(std::size_t row, std::size_t col) const {
    return operator()(row, col);
  }

private:
  std::vector<std::vector<Disk>> m_data;
  constexpr static std::size_t m_size = 8;
};

std::ostream& operator<<(std::ostream& os, const Disk disk) {
  switch (disk) {
    case Disk::Empty: os << ' '; break;
    case Disk::White: os << "\u25e6"; break;
    case Disk::Black: os << "\u00b7"; break;
  }
  return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
  for (const T& item : vector) {
    os << item << ", ";
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
  os << "Board shape: " << board.size() << " x " << board.size() << '\n';
  for (const auto& row : board.data()) {
    os << row << '\n';
  }
  return os;
}

class Player {
public:

private:
  std::string m_name;
};

int main() {
  Board board;
  std::cout << board;
}
