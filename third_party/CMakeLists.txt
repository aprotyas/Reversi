# ImGui integration
include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY    https://github.com/ocornut/imgui.git
  GIT_REMOTE_NAME   imgui_remote
  GIT_TAG           imgui_remote/master
  GIT_PROGRESS      TRUE
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
)

FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
)

target_include_directories(imgui
  PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
  ${imgui_SOURCE_DIR}/include
  PRIVATE
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIRS}
)

# CMAKE_DL_LIBS required for the OpenGL loader
target_link_libraries(imgui PUBLIC OpenGL::GL ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS})
