#include <cstddef>
#include <iostream>
#include <type_traits>

#include "othello/othello.hpp"

struct GUI {};
struct TUI {};

template <typename CanvasType,
          typename = std::enable_if_t<std::is_same_v<CanvasType, TUI> ||
                                      std::is_same_v<CanvasType, GUI>>>
struct CanvasImpl {
  static void show_intro();
  static void show_rules();
};

template <>
struct CanvasImpl<TUI> {
  static void show_intro() {
    std::cout << " ------------------------------------------------------------"
                 "------ \n";
    std::cout << " |                      WELCOME TO OTHELLO                   "
                 "     | \n";
    std::cout << " ------------------------------------------------------------"
                 "------ \n";
  }

  static void show_rules() {}
};

int main() {
#ifdef GUI_CANVAS
  using Canvas = CanvasImpl<GUI>;
#elif defined TUI_CANVAS
  using Canvas = CanvasImpl<TUI>;
#endif
  Canvas::show_intro();

  Othello othello;

  // othello.init(player_names);

  while (othello.game_over()) {
    // Show game state
    // Get (and validate?) user inputs
    // Update game state -- othello.update(player_moves)
    std::cout << othello << '\n';
    break;
  }
  return 0;
}
