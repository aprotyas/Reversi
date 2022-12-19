#include <iostream>

#include "othello/othello.hpp"

int main() {
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
