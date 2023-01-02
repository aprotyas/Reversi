cmake_minimum_required(VERSION 3.22)
project(othello
  VERSION 0.1
  DESCRIPTION "A C++ implementation of the 2-player board game, Othello"
  HOMEPAGE_URL "https://github.com/aprotyas/othello"
  LANGUAGES CXX
)

# Boost.Outcome
find_package(fmt REQUIRED)
find_package(Boost 1.74 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
add_subdirectory(third_party)

set(wrappers_srcs
  src/wrappers/imgui.cpp
  src/wrappers/sdl_gl.cpp
)
add_library(wrappers EXCLUDE_FROM_ALL ${wrappers_srcs})
target_include_directories(wrappers PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)
target_link_libraries(wrappers
  PUBLIC
  Boost::boost
  fmt::fmt
  imgui
  OpenGL::GL
  ${SDL2_LIBRARIES}
)
set_target_properties(wrappers PROPERTIES
  CXX_CLANG_TIDY clang-tidy -header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/wrappers/*.hpp --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
  CXX_EXTENSIONS OFF
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON
)

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
  Boost::boost
  fmt::fmt
  imgui
  OpenGL::GL
  ${SDL2_LIBRARIES}
  wrappers
)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  include(CTest)
endif()
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} AND BUILD_TESTING)
  add_subdirectory(test)
endif()
