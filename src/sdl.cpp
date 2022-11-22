#include <SDL.h>
#include <frozen/unordered_map.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace fs = std::filesystem;

enum class KeyPressSurfaces { DEFAULT, UP, DOWN, LEFT, RIGHT };

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
  if (auto* surface = SDL_LoadBMP(surface_path.c_str())) {
    return SurfacePtr{surface};
  }
  std::cerr << "Unable to load image " << surface_path
            << "! SDL Error: " << SDL_GetError() << '\n';
  return nullptr;
}

[[nodiscard]] SurfacePtr load_opt_optimized_surface(const fs::path& surface_path, const SDL_PixelFormat* format) {
  if (auto maybe_surface = load_opt_surface(surface_path)) {
    if (auto* optimized_surface = SDL_ConvertSurface(maybe_surface.get(), format, 0)) {
      return SurfacePtr{optimized_surface};
    }
    std::cerr << "Unable to optimize surface " << surface_path
              << "! SDL Error: " << SDL_GetError() << '\n';
  }
  return nullptr;
}

[[nodiscard]] std::optional<std::unordered_map<KeyPressSurfaces, SurfacePtr>>
make_opt_optimized_surface_map(const SDL_PixelFormat* format) {
  static constexpr frozen::unordered_map<KeyPressSurfaces, std::string_view, 5>
      surface_resource_map = {
          {KeyPressSurfaces::DEFAULT, "resources/press.bmp"},
          {KeyPressSurfaces::UP, "resources/up.bmp"},
          {KeyPressSurfaces::DOWN, "resources/down.bmp"},
          {KeyPressSurfaces::LEFT, "resources/left.bmp"},
          {KeyPressSurfaces::RIGHT, "resources/right.bmp"},
      };

  std::unordered_map<KeyPressSurfaces, SurfacePtr> surface_map;
  for (const auto [keypress, surface_path] : surface_resource_map) {
    if (auto maybe_surface = load_opt_optimized_surface(fs::path{surface_path}, format)) {
      if (!surface_map
               .insert(std::make_pair(keypress, std::move(maybe_surface)))
               .second) {
        return {};
      }
    } else {
      return {};
    }
  }
  return surface_map;
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SLD could not initialize! SDL_Error: " << SDL_GetError()
              << '\n';
    std::exit(-1);
  }

  std::optional<App> maybe_app = make_opt_sdl_app();
  if (!maybe_app) {
    std::exit(-1);
  }

  const auto maybe_surface_map = make_opt_optimized_surface_map(maybe_app->surface()->format);
  if (!maybe_surface_map) {
    std::exit(-1);
  }

  SDL_Event e;
  KeyPressSurfaces active_keypress = KeyPressSurfaces::DEFAULT;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_UP:
            active_keypress = KeyPressSurfaces::UP;
            break;
          case SDLK_DOWN:
            active_keypress = KeyPressSurfaces::DOWN;
            break;
          case SDLK_LEFT:
            active_keypress = KeyPressSurfaces::LEFT;
            break;
          case SDLK_RIGHT:
            active_keypress = KeyPressSurfaces::RIGHT;
            break;
          default:
            active_keypress = KeyPressSurfaces::DEFAULT;
            break;
        }
      }
    }

    SDL_BlitSurface(maybe_surface_map->at(active_keypress).get(), nullptr,
                    maybe_app->surface(), nullptr);
    SDL_UpdateWindowSurface(maybe_app->window());
  }

  SDL_Quit();
}
