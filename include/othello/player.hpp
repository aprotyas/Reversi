#pragma once

#include <string>
#include <string_view>

class Player {
 public:
  std::string_view name() const;

  std::size_t score() const;
  std::size_t& score();

 private:
  std::string m_name;
  std::size_t m_score{};
};
