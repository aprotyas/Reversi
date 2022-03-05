cmake_minimum_required(VERSION 3.16)

project(reversi
  VERSION 0.1
  DESCRIPTION "A C++ implementation of the 2-player board game, Reversi"
  HOMEPAGE_URL "https://github.com/aprotyas/reversi"
  LANGUAGES CXX
)

set(_lib_srcs
  src/${PROJECT_NAME}/board.cpp
  src/${PROJECT_NAME}/disk.cpp
  src/${PROJECT_NAME}/player.cpp
)
add_library(${PROJECT_NAME}_lib ${_lib_srcs})
target_include_directories(${PROJECT_NAME}_lib PUBLIC include/${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_lib PROPERTIES
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
