#include <SDL2/SDL.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "wrappers/imgui.hpp"
#include "wrappers/sdl_gl.hpp"
#include "wrappers/wrappers.hpp"

namespace gt {
using ::testing::_;
} // namespace gt

struct MockDeleterInterface : public wrappers::detail::DeleterInterface {
  ~MockDeleterInterface() override = default;
  MockDeleterInterface() = default;
  MockDeleterInterface(const MockDeleterInterface &) = default;
  MockDeleterInterface(MockDeleterInterface &&) noexcept = default;
  MockDeleterInterface &operator=(const MockDeleterInterface &) = default;
  MockDeleterInterface &operator=(MockDeleterInterface &&) noexcept = default;
  MOCK_METHOD(void, sdl_destroy_window, (SDL_Window *), (override));
};
using MockDeleter = wrappers::detail::DeleterImpl<MockDeleterInterface>;

class DeleterTest : public ::testing::Test {
protected:
  void SetUp() override {
    regular_sdl_window =
        SDL_CreateWindow("", 0, 0, 0, 0, static_cast<SDL_WindowFlags>(0));
  }

  void TearDown() override {
    if (regular_sdl_window != nullptr) {
      wrappers::Deleter{}(regular_sdl_window);
    }
  }

  // NOLINTBEGIN(*-non-private-member-variables-in-classes)
  MockDeleter deleter{};
  SDL_Window *null_sdl_window{nullptr};
  SDL_Window *regular_sdl_window{nullptr};
  // NOLINTEND(*-non-private-member-variables-in-classes)
};

TEST_F(DeleterTest, HandlesNullResource) {
  EXPECT_CALL(deleter.get_deleter(), sdl_destroy_window(gt::_)).Times(0);
  deleter(null_sdl_window);
}

TEST_F(DeleterTest, HandlesRegularResource) {
  EXPECT_CALL(deleter.get_deleter(), sdl_destroy_window(regular_sdl_window))
      .Times(1);
  deleter(regular_sdl_window);
}

// TEST(Imgui, Text) {

// }

// TEST(SDLCreateWindow, HandleNullWindow) {
//   // Mock the free function to always return nullptr window to see if the
//   correct result is provided
// }

// TEST(SDLCreateWindow, HandleRegularWindow) {

// }

// TEST(SDLInit, HandleIncorrectInit) {
//   // Mock the free function to always return non-zero to see if the correct
//   result is provided
// }

// TEST(SDLInit, HandleRegularInit) {

// }

// TEST(SDLGLContext, Context) {

// }

// TEST(SDLGLWrapperTest, GLSLVersion) {

// }

// TEST(SDLGLWrapperTest, MakeCurrent) {

// }

// TEST(SDLGLWrapperTest, SetSwapInterval) {

// }
