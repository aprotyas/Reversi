cmake_minimum_required(VERSION 3.21)

project(othello
  VERSION 0.1
  DESCRIPTION "A C++ implementation of the 2-player board game, Othello"
  HOMEPAGE_URL "https://github.com/aprotyas/othello"
  LANGUAGES CXX
)

set(${PROJECT_NAME}_srcs
  src/${PROJECT_NAME}/board.cpp
  src/${PROJECT_NAME}/disk.cpp
  src/${PROJECT_NAME}/player.cpp
  src/${PROJECT_NAME}/othello.cpp
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

option(BUILD_GUI "Build the GUI application" OFF)
option(BUILD_TUI "Build the TUI application" ON)

function(multi_option variable docstring values)
  set(possible_values "${values}")
  if(ARGC GREATER "3")
    list(LENGTH ARGN length)
    list(PREPEND ARGN "${values}")
    list(GET ARGN "${length}" values)
    set(possible_values "${ARGN}")
  endif()

  option("${variable}" "${docstring}" "${values}")
  set_property(CACHE "${variable}" PROPERTY STRINGS "${possible_values}")
  if(NOT "${${variable}}" IN_LIST possible_values)
    message(FATAL_ERROR "The variable \"${variable}\" has an unknown value: ${${variable}}\nPossible values are: ${possible_values}")
  endif()
endfunction()

multi_option(BUILD_APP_VERSION "Which version of the app to build; valid options: TUI, GUI" TUI GUI)

# SDL2-OpenGL integration
if("${BUILD_APP_VERSION}" STREQUAL GUI)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(third_party)
endif()

add_executable(${PROJECT_NAME}_app src/${PROJECT_NAME}_app.cpp)
target_compile_definitions(${PROJECT_NAME}_app
  PRIVATE
  "$<$<STREQUAL:${BUILD_APP_VERSION},GUI>:GUI_CANVAS>"
  "$<$<STREQUAL:${BUILD_APP_VERSION},TUI>:TUI_CANVAS>"
)
set_target_properties(${PROJECT_NAME}_app PROPERTIES
  CXX_CLANG_TIDY clang-tidy -header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON
  OUTPUT_NAME ${PROJECT_NAME}
)
target_include_directories(${PROJECT_NAME}_app
  PRIVATE
  "$<$<STREQUAL:${BUILD_APP_VERSION},GUI>:${imgui_SOURCE_DIR};${imgui_SOURCE_DIR}/backends;${imgui_SOURCE_DIR}/include;${OPENGL_INCLUDE_DIR};${SDL2_INCLUDE_DIRS}>"
)
target_link_libraries(${PROJECT_NAME}_app
  PRIVATE
  ${PROJECT_NAME}
  "$<$<STREQUAL:${BUILD_APP_VERSION},GUI>:imgui;OpenGL::GL;${SDL2_LIBRARIES}>"
)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  include(CTest)
endif()
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} AND BUILD_TESTING)
  add_subdirectory(test)
endif()
