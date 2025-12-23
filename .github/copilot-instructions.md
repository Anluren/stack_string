# Copilot Instructions for stack_string

## Project Overview
- **stack_string** is a C++17 header-only library for efficient, stack-allocated, fixed-capacity strings.
- Two main components:
  - `StackString<N>`: A `std::string`-like class with stack-based, fixed-size storage.
  - `FixedBufAllocator`: A custom allocator for `std::basic_string` using a user-supplied buffer (stack or otherwise).
- No heap allocations; all memory is stack-based for performance-critical use.

## Key Files & Structure
- `include/stack_string.hpp`: Main `StackString` implementation.
- `include/fixed_buf_allocator.hpp`: `FixedBufAllocator` implementation.
- `examples/`: Usage examples, including `basic_usage.cpp`, `buffer_allocator_usage.cpp`, and `mixed_usage.cpp`.
- `CMakeLists.txt`: Top-level build configuration.

## Build & Test Workflow
- Requires C++17 and CMake 3.10+.
- Standard build:
  ```bash
  mkdir build && cd build
  cmake ..
  cmake --build .
  # Run examples:
  ./examples/basic_usage
  ./examples/fixed_buf_allocator_usage
  ./examples/mixed_usage
  ```
- No unit tests are present; examples serve as functional tests.

## Usage Patterns & Conventions
- Prefer `StackString<N>` for most stack-based string needs; use `FixedBufAllocator` for `std::string` compatibility.
- All string operations are silent on overflow (truncate, no exceptions).
- Use stream-style (`operator<<`) or variadic constructor for efficient string building.
- Implicit conversion to `const char*` and `std::string_view` is supported for interoperability.
- One `FixedBufAllocator` per string instance/buffer.

## Integration
- As a CMake subdirectory: `add_subdirectory(stack_string)` and `target_link_libraries(your_target PRIVATE stack_string)`.
- As an installed package: `find_package(StackString REQUIRED)` and `target_link_libraries(your_target PRIVATE StackString::stack_string)`.

## Design Notes
- No exceptions: all errors (e.g., overflow) are handled by truncation.
- Null-termination is always maintained for C string compatibility.
- Integer-to-string conversion uses `std::to_chars` for efficiency.

## Examples
- See `examples/` and README for idiomatic usage.
- Typical pattern:
  ```cpp
  StackString<128> msg;
  msg << "User " << 42 << " has " << 5 << " messages";
  std::cout << msg << std::endl;
  ```

---
_If you are an AI agent, follow these conventions and reference the README for further details. Update this file if project structure or conventions change._
