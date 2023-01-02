#include <SDL2/SDL.h>

#include "wrappers/wrappers.hpp"

namespace wrappers {
void Deleter::operator()(SDL_Window *window) {
  if (window != nullptr) {
    SDL_DestroyWindow(window);
  }
}
} // namespace wrappers
