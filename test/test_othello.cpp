#include <gtest/gtest.h>

#include <cstddef>

class TestOthello : public ::testing::Test {};

TEST_F(TestOthello, SanityCheck) {
  { EXPECT_EQ(true, true); }
}
