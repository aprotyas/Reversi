#include <SDL.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

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

  explicit App(WindowPtr window) : window_ {std::move(window)} {}

  App(const App& other) = delete;
  App& operator=(const App& other) = delete;

  App(App&& other) noexcept : window_ {std::move(other.window_)} {}
  App& operator=(App&& other) noexcept {
    window_ = std::move(other.window_);
    return *this;
  }

  [[nodiscard]] const SDL_Window* window() const { return window_.get(); }
  [[nodiscard]] SDL_Window* window() { return window_.get(); }
  [[nodiscard]] const SDL_Surface* surface() const { return SDL_GetWindowSurface(window_.get()); }
  [[nodiscard]] SDL_Surface* surface() { return SDL_GetWindowSurface(window_.get()); }

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
  std::cout << "SDL window could not be created! SDL_Error: " << SDL_GetError()
            << '\n';
  return {};
}

[[nodiscard]] std::optional<SurfacePtr> make_opt_demo_surface() {
  if (auto* surface = SDL_LoadBMP("resources/x.bmp")) {
    return {SurfacePtr{surface}};
  }
  std::cout << "SDL surface could not be created! SDL_Error: " << SDL_GetError()
            << '\n';
  return {};
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SLD could not initialize! SDL_Error: " << SDL_GetError()
              << '\n';
    std::exit(-1);
  }

  App app;
  if (std::optional<App> maybe_app = make_opt_sdl_app()) {
    app = std::move(maybe_app.value());
  } else {
    std::exit(-1);
  }

  SurfacePtr demo_surface = nullptr;
  if (auto maybe_demo_surface = make_opt_demo_surface()) {
    demo_surface = std::move(maybe_demo_surface.value());
  } else {
    std::exit(-1);
  }

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    SDL_BlitSurface(demo_surface.get(), nullptr, app.surface(), nullptr);
    SDL_UpdateWindowSurface(app.window());
  }

  SDL_Quit();
}
