#pragma once

#include <SDL2/SDL.h>
#include <boost/outcome.hpp>
#ifdef IMGUI_IMPL_OPENGL_ES2
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#include "wrappers/wrappers.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace {
namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;
} // namespace

namespace wrappers::sdl {
using WindowPtr = std::unique_ptr<SDL_Window, wrappers::Deleter>;

[[nodiscard]] outcome::result<WindowPtr, std::string_view>
create_window(const std::string &title, const int xpos, const int ypos,
              const int width, const int height,
              const SDL_WindowFlags window_options);

[[nodiscard]] outcome::result<void, std::string_view> init();

namespace gl {
struct Context {
  Context(const Context &) = delete;
  Context &operator=(const Context &) = delete;
  Context(Context &&) = delete;
  Context &operator=(Context &&) = delete;

  explicit Context(SDL_GLContext &&context);
  ~Context();

  [[nodiscard]] SDL_GLContext get() const;

private:
  SDL_GLContext context_;
};

[[nodiscard]] constexpr std::string_view glsl_version() {
#ifdef IMGUI_IMPL_OPENGL_ES2
  // GL ES 2.0 + GLSL 100
  return "#version 100";
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  return "#version 150";
#else
  // GL 3.0 + GLSL 130
  return "#version 130";
#endif
}

[[nodiscard]] outcome::result<void, std::string_view>
make_current(SDL_Window *window, SDL_GLContext gl_context);

[[nodiscard]] outcome::result<void, std::string_view>
set_swap_interval(const int interval);

void context_version_init();
} // namespace gl
} // namespace wrappers::sdl
