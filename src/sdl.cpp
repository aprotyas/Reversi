#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <frozen/unordered_map.h>

#include <boost/scope_exit.hpp>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace fs = std::filesystem;

struct SdlDeleter {
  void operator()(SDL_Window* window) {
    if (window != nullptr) {
      SDL_DestroyWindow(window);
    }
  }

  void operator()(SDL_Surface* surface) {
    if (surface != nullptr) {
      SDL_FreeSurface(surface);
    }
  }
};

using WindowPtr = std::unique_ptr<SDL_Window, SdlDeleter>;
using SurfacePtr = std::unique_ptr<SDL_Surface, SdlDeleter>;

namespace init_state {
bool sdl{false};
bool sdl_image{false};
}  // namespace init_state

class App {
 public:
  App() = default;
  ~App() = default;

  explicit App(WindowPtr window) : window_{std::move(window)} {}

  App(const App& other) = delete;
  App& operator=(const App& other) = delete;

  App(App&& other) noexcept : window_{std::move(other.window_)} {}
  App& operator=(App&& other) noexcept {
    window_ = std::move(other.window_);
    return *this;
  }

  [[nodiscard]] const SDL_Window* window() const { return window_.get(); }
  [[nodiscard]] SDL_Window* window() { return window_.get(); }
  [[nodiscard]] const SDL_Surface* surface() const {
    return SDL_GetWindowSurface(window_.get());
  }
  [[nodiscard]] SDL_Surface* surface() {
    return SDL_GetWindowSurface(window_.get());
  }

  static constexpr uint32_t screen_width = 640UL;
  static constexpr uint32_t screen_height = 480UL;

 private:
  WindowPtr window_ = nullptr;
};

[[nodiscard]] std::optional<App> make_opt_sdl_app() {
  if (auto* window = SDL_CreateWindow(
          "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
          App::screen_width, App::screen_height, SDL_WINDOW_SHOWN)) {
    return App{WindowPtr{window}};
  }
  std::cerr << "SDL window could not be created! SDL_Error: " << SDL_GetError()
            << '\n';
  return {};
}

[[nodiscard]] SurfacePtr load_opt_surface(const fs::path& surface_path) {
  if (auto* surface = IMG_Load(surface_path.c_str())) {
    return SurfacePtr{surface};
  }
  std::cerr << "Unable to load image " << surface_path
            << "! SDL Error: " << SDL_GetError() << '\n';
  return nullptr;
}

int main() {
  BOOST_SCOPE_EXIT(void) {
    if (init_state::sdl) {
      SDL_Quit();
    }
    if (init_state::sdl_image) {
      IMG_Quit();
    }
  }
  BOOST_SCOPE_EXIT_END

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << '\n';
    return -1;
  }
  init_state::sdl = true;

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
    std::cerr << "SDL_image could not initialize! SDL_image error: "
              << IMG_GetError() << '\n';
    return -1;
  }
  init_state::sdl_image = true;

  std::optional<App> maybe_app = make_opt_sdl_app();
  if (!maybe_app) {
    return -1;
  }

  if (SurfacePtr png_surface{load_opt_surface("resources/loaded.png")}) {
    SDL_BlitSurface(png_surface.get(), nullptr, maybe_app->surface(), nullptr);
    SDL_UpdateWindowSurface(maybe_app->window());
  } else {
    return -1;
  }

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
  }
}
