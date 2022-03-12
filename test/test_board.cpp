#include <cstddef>

#include <gtest/gtest.h>
#include "reversi/board.hpp"

class TestBoard : public ::testing::Test
{
protected:
  TestBoard() = default;
  Board board;
  constexpr static std::size_t expected_board_size = 8;
};

TEST_F(TestBoard, size) {
  {
    EXPECT_EQ(board.size(), TestBoard::expected_board_size) << "Board size is not " << TestBoard::expected_board_size;
  }
}
