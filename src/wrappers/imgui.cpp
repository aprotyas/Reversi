#include "imgui.h"

#include "wrappers/imgui.hpp"

#include <string>

namespace wrappers::imgui {
void text(const std::string &str) { ImGui::TextUnformatted(str.c_str()); }
} // namespace wrappers::imgui
