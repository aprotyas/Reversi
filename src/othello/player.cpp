#include "othello/player.hpp"

#include <string_view>

std::string_view Player::name() const { return m_name; }

std::size_t Player::score() const { return m_score; }
std::size_t& Player::score() { return m_score; }
