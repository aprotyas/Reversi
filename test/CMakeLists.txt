include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY  https://github.com/google/googletest.git
  GIT_TAG         release-1.11.0
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(test_othello test_othello.cpp)
set_target_properties(test_othello PROPERTIES
  CXX_CLANG_TIDY clang-tidy -header-filter=${CMAKE_SOURCE_DIR}/include/*.hpp --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
  CXX_EXTENSIONS OFF
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  EXPORT_COMPILE_COMMANDS ON
)
target_link_libraries(test_othello gtest_main)
gtest_discover_tests(test_othello)
