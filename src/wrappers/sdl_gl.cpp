#include <SDL2/SDL.h>
#include <boost/outcome.hpp>
#ifdef IMGUI_IMPL_OPENGL_ES2
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#include "wrappers/sdl_gl.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

namespace wrappers::sdl {
[[nodiscard]] outcome::result<WindowPtr, std::string_view>
create_window(const std::string &title, const int xpos, const int ypos,
              const int width, const int height,
              const SDL_WindowFlags window_options) {
  if (auto *window = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height,
                                      window_options)) {
    return WindowPtr{window};
  }
  return {SDL_GetError()};
}

[[nodiscard]] outcome::result<void, std::string_view> init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    return {SDL_GetError()};
  }
  return outcome::success();
}

namespace gl {
Context::Context(SDL_GLContext &&context) : context_{context} {}

Context::~Context() { SDL_GL_DeleteContext(context_); }

[[nodiscard]] SDL_GLContext Context::get() const { return context_; }

[[nodiscard]] outcome::result<void, std::string_view>
make_current(SDL_Window *window, SDL_GLContext gl_context) {
  if (SDL_GL_MakeCurrent(window, gl_context) == 0) {
    return outcome::success();
  }
  return {SDL_GetError()};
}

[[nodiscard]] outcome::result<void, std::string_view>
set_swap_interval(const int interval) {
  if (SDL_GL_SetSwapInterval(interval) == -1) {
    return {SDL_GetError()};
  }
  return outcome::success();
}

void context_version_init() {
// Decide GL+GLSL versions
#ifdef IMGUI_IMPL_OPENGL_ES2
  // GL ES 2.0 + GLSL 100
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
}
} // namespace gl
} // namespace wrappers::sdl
