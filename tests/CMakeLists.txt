# CMake configuration for GoogleTest

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(stack_string_tests
  stack_string_tests.cpp
  fixed_buf_allocator_tests.cpp
)

target_include_directories(stack_string_tests PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(stack_string_tests
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(stack_string_tests)
