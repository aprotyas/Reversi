cmake_minimum_required(VERSION 3.21)

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

# SDL2-OpenGL integration
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(third_party)

add_executable(${PROJECT_NAME}_app src/${PROJECT_NAME}_app.cpp)
set_target_properties(${PROJECT_NAME}_app PROPERTIES
  CXX_CLANG_TIDY clang-tidy -header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON
  OUTPUT_NAME ${PROJECT_NAME}
)
target_include_directories(${PROJECT_NAME}_app PRIVATE
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
  ${imgui_SOURCE_DIR}/include
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_app PRIVATE ${PROJECT_NAME} imgui OpenGL::GL ${SDL2_LIBRARIES})

add_executable(sdl src/sdl.cpp)
set_target_properties(sdl PROPERTIES
  CXX_CLANG_TIDY clang-tidy -header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON
)
target_include_directories(sdl PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(sdl PRIVATE ${SDL2_LIBRARIES} frozen::frozen)

add_custom_command(TARGET sdl POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:sdl>/resources)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  include(CTest)
endif()
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} AND BUILD_TESTING)
  add_subdirectory(test)
endif()
