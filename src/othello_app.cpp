#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <SDL2/SDL.h>
#include <boost/outcome.hpp>
#include <boost/scope_exit.hpp>
#include <fmt/core.h>
#ifdef IMGUI_IMPL_OPENGL_ES2
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#include "wrappers/imgui.hpp"
#include "wrappers/sdl_gl.hpp"
#include "wrappers/wrappers.hpp"

#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

using namespace std::chrono_literals;

int main() {
  // Setup SDL
  if (const auto init_result = wrappers::sdl::init();
      init_result.has_failure()) {
    std::cerr << init_result.error() << '\n';
    return -1;
  }
  BOOST_SCOPE_EXIT(void) { SDL_Quit(); }
  BOOST_SCOPE_EXIT_END

  wrappers::sdl::gl::context_version_init();

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  constexpr const std::size_t depth_size = 24;
  constexpr const std::size_t stencil_size = 8;
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth_size);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencil_size);

  auto window_flags = static_cast<SDL_WindowFlags>(
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  wrappers::sdl::WindowPtr window{nullptr};

  constexpr const std::size_t window_width = 1280;
  constexpr const std::size_t window_height = 720;
  if (auto maybe_window = wrappers::sdl::create_window(
          "Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
          window_width, window_height, window_flags)) {
    window = std::move(maybe_window.value());
  } else {
    std::cerr << maybe_window.error() << '\n';
    return -1;
  }

  auto gl_context =
      wrappers::sdl::gl::Context{SDL_GL_CreateContext(window.get())};

  if (const auto result =
          wrappers::sdl::gl::make_current(window.get(), gl_context.get());
      result.has_failure()) {
    std::cerr << result.error() << '\n';
    return -1;
  }
  if (const auto result = wrappers::sdl::gl::set_swap_interval(1);
      result.has_failure()) { // enable vsync
    std::cerr << result.error() << '\n';
    return -1;
  }

  // Setup Dear ImGui context and platform/renderer backends
  IMGUI_CHECKVERSION();
  if (ImGui::CreateContext() == nullptr) {
    std::cerr << "ImGui context creation failed" << '\n';
    return -1;
  }
  if (!ImGui_ImplSDL2_InitForOpenGL(window.get(), gl_context.get())) {
    std::cerr << "ImGui SDL backend init failed" << '\n';
    return -1;
  }
  if (!ImGui_ImplOpenGL3_Init(wrappers::sdl::gl::glsl_version().data())) {
    std::cerr << "ImGui OpenGL3 backend init failed" << '\n';
    return -1;
  }

  BOOST_SCOPE_EXIT(void) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }
  BOOST_SCOPE_EXIT_END

  ImGuiIO &imgui_io = ImGui::GetIO();

  bool show_demo_window = true;
  bool done = false;
  ImVec4 clear_color = ImVec4(1.00F, 1.00F, 1.00F, 1.00F);

  // main loop
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) {
        done = true;
      }
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(window.get())) {
        done = true;
      }
    }

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to create a named window.
    {
      static float slider = 0.0F;
      static int counter = 0;

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                     // and append into it.

      ImGui::TextUnformatted(
          "This is some useful text."); // Display some text (you can use a
                                        // format strings too)
      ImGui::Checkbox(
          "Demo Window",
          &show_demo_window); // Edit bools storing our window open/close state

      ImGui::SliderFloat("float", &slider, 0.0F,
                         1.0F); // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color",
                        &clear_color.x); // Edit 3 floats representing a color

      if (ImGui::Button(
              "Button")) { // Buttons return true when clicked (most widgets
                           // return true when edited/activated)
        counter++;
      }
      ImGui::SameLine();
      wrappers::imgui::text(fmt::format("counter = {}", counter));

      constexpr const std::chrono::seconds one_second = 1s;
      wrappers::imgui::text(fmt::format(
          "Application average {:.3f} ms/frame ({:.1f} FPS)",
          std::chrono::duration_cast<std::chrono::milliseconds>(one_second)
                  .count() /
              ImGui::GetIO().Framerate,
          ImGui::GetIO().Framerate));
      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, static_cast<int>(imgui_io.DisplaySize.x),
               static_cast<int>(imgui_io.DisplaySize.y));
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window.get());
  }
}
