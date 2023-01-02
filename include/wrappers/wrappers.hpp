#pragma once

#include <SDL2/SDL.h>

#include <concepts>

namespace wrappers {
namespace detail {
struct DeleterInterface {
  DeleterInterface() = default;
  virtual ~DeleterInterface() = default;
  DeleterInterface(const DeleterInterface &) = default;
  DeleterInterface(DeleterInterface &&) noexcept = default;
  DeleterInterface &operator=(const DeleterInterface &) = default;
  DeleterInterface &operator=(DeleterInterface &&) noexcept = default;
  virtual void sdl_destroy_window(SDL_Window *) = 0;
};

struct ProdDeleterInterface : public DeleterInterface {
  virtual ~ProdDeleterInterface() = default;
  virtual void sdl_destroy_window(SDL_Window *window) override {
    if (window != nullptr) {
      SDL_DestroyWindow(window);
    }
  }
};

// This interface has been used to allow static dependency injection during
// testing, since most (all?) underlying "deleter" functions are free functions
// from C-APIs
template <std::derived_from<DeleterInterface> Deleter = ProdDeleterInterface>
struct DeleterImpl {
  void operator()(SDL_Window *window) {
    if (window != nullptr) {
      deleter.sdl_destroy_window(window);
    }
  }

  Deleter &get_deleter() { return deleter; }
  const Deleter &get_deleter() const { return deleter; }

private:
  Deleter deleter{};
};
} // namespace detail

using Deleter = detail::DeleterImpl<>;
} // namespace wrappers
