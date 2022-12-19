#include <gtest/gtest.h>

#include <cstddef>

#include "othello/board.hpp"

class TestBoard : public ::testing::Test {
 protected:
  TestBoard() = default;
  Board board;
  static constexpr const std::size_t expected_board_size{8UL};
};

TEST_F(TestBoard, size) {
  {
    EXPECT_EQ(Board::size, TestBoard::expected_board_size)
        << "Board size is not " << TestBoard::expected_board_size;
  }
}
