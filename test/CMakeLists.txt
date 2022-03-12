include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY  https://github.com/google/googletest.git
  GIT_TAG         release-1.11.0
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(test_board test_board.cpp)
set_target_properties(test_board PROPERTIES
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
target_link_libraries(test_board gtest_main ${PROJECT_NAME})
gtest_discover_tests(test_board)
