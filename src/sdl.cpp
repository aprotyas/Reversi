#include <SDL.h>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>

struct SdlDeleter {
  void operator()(SDL_Window* window) {
    if (window != nullptr) {
      SDL_DestroyWindow(window);
    }
  }
};

struct App {
  using WindowPtr = std::unique_ptr<SDL_Window, SdlDeleter>;
  WindowPtr window;

  static constexpr uint32_t screen_width = 640UL;
  static constexpr uint32_t screen_height = 480UL;
};

namespace {
constexpr uint16_t white = 0xFF;
}

[[nodiscard]] std::optional<App> make_opt_sdl_app() {
  if (SDL_Window* window = SDL_CreateWindow(
          "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
          App::screen_width, App::screen_height, SDL_WINDOW_SHOWN)) {
    return std::make_optional<App>({App::WindowPtr{window}});
  }
  std::cout << "SDL window could not be created! SDL_Error: " << SDL_GetError()
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

  {
    SDL_Surface* surface = SDL_GetWindowSurface(app.window.get());
    SDL_FillRect(surface, nullptr,
                 SDL_MapRGB(surface->format, ::white, ::white, ::white));
    SDL_UpdateWindowSurface(app.window.get());
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

  SDL_Quit();
}
