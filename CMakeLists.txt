cmake_minimum_required(VERSION 3.16)

project(reversi
  VERSION 0.1
  DESCRIPTION "A C++ implementation of the 2-player board game, Reversi"
  HOMEPAGE_URL "https://github.com/aprotyas/reversi"
  LANGUAGES CXX
)

add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
