#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <frozen/unordered_map.h>

#include <boost/scope_exit.hpp>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace fs = std::filesystem;

namespace init_state {
bool sdl{false};
bool sdl_image{false};
}  // namespace init_state

namespace sdl {
struct Deleter {
  void operator()(SDL_Window* window) {
    if (window != nullptr) {
      SDL_DestroyWindow(window);
    }
  }

  void operator()(SDL_Renderer* renderer) {
    if (renderer != nullptr) {
      SDL_DestroyRenderer(renderer);
    }
  }

  void operator()(SDL_Surface* surface) {
    if (surface != nullptr) {
      SDL_FreeSurface(surface);
    }
  }

  void operator()(SDL_Texture* texture) {
    if (texture != nullptr) {
      SDL_DestroyTexture(texture);
    }
  }
};

using WindowPtr = std::unique_ptr<SDL_Window, sdl::Deleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, sdl::Deleter>;
using SurfacePtr = std::unique_ptr<SDL_Surface, sdl::Deleter>;
using TexturePtr = std::unique_ptr<SDL_Texture, sdl::Deleter>;

[[nodiscard]] sdl::WindowPtr create_window(
    const std::string& title, const int xpos, const int ypos, const int width,
    const int height, const SDL_WindowFlags window_options) {
  if (auto* window = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height,
                                      window_options)) {
    return sdl::WindowPtr{window};
  }
  std::cerr << "SDL window could not be created! SDL_Error: " << SDL_GetError()
            << '\n';
  return nullptr;
}

[[nodiscard]] sdl::RendererPtr create_renderer(
    SDL_Window& window,
    const SDL_RendererFlags renderer_options = SDL_RENDERER_ACCELERATED) {
  if (auto* renderer = SDL_CreateRenderer(&window, -1, renderer_options)) {
    return sdl::RendererPtr{renderer};
  }
  return nullptr;
}

[[nodiscard]] sdl::SurfacePtr load_surface(const fs::path& surface_path) {
  if (auto* surface = IMG_Load(surface_path.c_str())) {
    return sdl::SurfacePtr{surface};
  }
  std::cerr << "Unable to load image " << surface_path
            << "! SDL Error: " << SDL_GetError() << '\n';
  return nullptr;
}

[[nodiscard]] sdl::TexturePtr load_texture(SDL_Renderer& renderer,
                                           const fs::path& texture_path) {
  if (auto surface = sdl::load_surface(texture_path)) {
    if (auto* texture =
            SDL_CreateTextureFromSurface(&renderer, surface.get())) {
      return sdl::TexturePtr{texture};
    }
  }
  std::cerr << "Unable to load texture " << texture_path
            << "! SDL Error: " << SDL_GetError() << '\n';
  return nullptr;
}

enum class Color : uint8_t {
  white,
  black,
  red,
};

namespace {
constexpr int white = 0xFF;
constexpr int black = 0x00;
}  // namespace

namespace std {
[[noreturn]] inline void unreachable() {
#ifdef __GNUC__
  __builtin_unreachable();
#elif defined _MSC_VER
  __assume(false);
#endif
}
}  // namespace std

void set_render_draw_color(SDL_Renderer& renderer, const sdl::Color color) {
  const auto set_color_impl = [&renderer](const int r, const int g,
                                          const int b) {
    SDL_SetRenderDrawColor(&renderer, r, g, b, white);
  };
  switch (color) {
    case sdl::Color::white:
      set_color_impl(white, white, white);
      break;
    case sdl::Color::black:
      set_color_impl(black, black, black);
      break;
    case sdl::Color::red:
      set_color_impl(white, black, black);
      break;
    default:
      std::unreachable();
  }
}
}  // namespace sdl

class App {
 public:
  App() = delete;
  ~App() = default;

  explicit App(sdl::WindowPtr window = nullptr,
               sdl::RendererPtr renderer = nullptr)
      : window_{std::move(window)}, renderer_{std::move(renderer)} {}

  App(const App& other) = delete;
  App& operator=(const App& other) = delete;

  App(App&& other) noexcept
      : window_{std::move(other.window_)},
        renderer_{std::move(other.renderer_)} {}
  App& operator=(App&& other) noexcept {
    window_ = std::move(other.window_);
    renderer_ = std::move(other.renderer_);
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
  [[nodiscard]] const SDL_Renderer* renderer() const { return renderer_.get(); }
  [[nodiscard]] SDL_Renderer* renderer() { return renderer_.get(); }

  static constexpr uint32_t screen_width = 640UL;
  static constexpr uint32_t screen_height = 480UL;

 private:
  sdl::WindowPtr window_ = nullptr;
  sdl::RendererPtr renderer_ = nullptr;
};

[[nodiscard]] std::optional<App> make_app(
    const std::string& title, const int xpos = SDL_WINDOWPOS_UNDEFINED,
    const int ypos = SDL_WINDOWPOS_UNDEFINED,
    const SDL_WindowFlags window_options = SDL_WINDOW_SHOWN) {
  sdl::WindowPtr window = sdl::create_window(
      title, xpos, ypos, App::screen_width, App::screen_height, window_options);
  if (window == nullptr) {
    return {};
  }
  sdl::RendererPtr renderer = sdl::create_renderer(*window);
  if (renderer == nullptr) {
    return {};
  }
  return App{std::move(window), std::move(renderer)};
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

  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == SDL_FALSE) {
    std::cerr << "Warning: Linear texture filtering not enabled!";
  }

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
    std::cerr << "SDL_image could not initialize! SDL_image error: "
              << IMG_GetError() << '\n';
    return -1;
  }
  init_state::sdl_image = true;

  std::optional<App> maybe_app = make_app("SDL tutorial");
  if (!maybe_app) {
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

    sdl::set_render_draw_color(*(maybe_app->renderer()), sdl::Color::red);
    SDL_RenderClear(maybe_app->renderer());

    SDL_Rect fill_rect{App::screen_width / 4, App::screen_height / 4,
                       App::screen_width / 2, App::screen_height / 2};
    sdl::set_render_draw_color(*(maybe_app->renderer()), sdl::Color::white);
    SDL_RenderFillRect(maybe_app->renderer(), &fill_rect);

    SDL_RenderPresent(maybe_app->renderer());
  }
}
