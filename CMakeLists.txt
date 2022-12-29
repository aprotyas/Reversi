cmake_minimum_required(VERSION 3.22)
project(othello
  VERSION 0.1
  DESCRIPTION "A C++ implementation of the 2-player board game, Othello"
  HOMEPAGE_URL "https://github.com/aprotyas/othello"
  LANGUAGES CXX
)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
add_subdirectory(third_party)

add_executable(${PROJECT_NAME}_app src/${PROJECT_NAME}_app.cpp)
set_target_properties(${PROJECT_NAME}_app PROPERTIES
  CXX_CLANG_TIDY clang-tidy -header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
  CXX_EXTENSIONS OFF
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON
  OUTPUT_NAME ${PROJECT_NAME}
)
target_link_libraries(${PROJECT_NAME}_app
  PRIVATE
  imgui
  OpenGL::GL
  ${SDL2_LIBRARIES}
)
