# ImGui integration
include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY  https://github.com/ocornut/imgui.git
  GIT_TAG         v1.88
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_GetProperties(imgui)

if (NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp)

target_include_directories(imgui
  PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
  ${imgui_SOURCE_DIR}/include
  PRIVATE
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIRS})

target_link_libraries(imgui PUBLIC OpenGL::GL ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS})

# serge-sans-paille/frozen
FetchContent_Declare(
  frozen
  GIT_REPOSITORY    https://github.com/serge-sans-paille/frozen
  GIT_TAG           1.1.1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
)
FetchContent_MakeAvailable(frozen)
