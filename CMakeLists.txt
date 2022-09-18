cmake_minimum_required(VERSION 3.11)

project(reversi
  VERSION 0.1
  DESCRIPTION "A C++ implementation of the 2-player board game, Reversi"
  HOMEPAGE_URL "https://github.com/aprotyas/reversi"
  LANGUAGES CXX
)

set(${PROJECT_NAME}_srcs
  src/${PROJECT_NAME}/board.cpp
  src/${PROJECT_NAME}/disk.cpp
  src/${PROJECT_NAME}/player.cpp
  src/${PROJECT_NAME}/reversi.cpp
)
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_srcs})
target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_CLANG_TIDY clang-tidy -header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON
)

add_executable(${PROJECT_NAME}_app src/${PROJECT_NAME}_app.cpp)
set_target_properties(${PROJECT_NAME}_app PROPERTIES
  CXX_CLANG_TIDY clang-tidy -header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON
  OUTPUT_NAME ${PROJECT_NAME}
)
target_link_libraries(${PROJECT_NAME}_app PRIVATE ${PROJECT_NAME})

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  include(CTest)
endif()
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} AND BUILD_TESTING)
  add_subdirectory(test)
endif()
