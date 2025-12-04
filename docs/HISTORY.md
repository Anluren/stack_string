# Development History

This document captures the exact commands and requests during the development of the StackString library.

## Initial Creation

**Command 1:** `let's create a c++17 library which support std::string like struct which use preallocated space to hold const strings; it support appending various type of strings to it, including const char *, std::string_view, integer types`

**Command 2:** `let's run it`
```bash
cd /home/dzheng/workspace/stack_string
mkdir build && cd build
cmake ..
cmake --build .
./basic_usage
```

## Code Refinements

**Command 3:** `for class member, use m_ as prefix`
- Changed `data_` → `m_data`, `size_` → `m_size`

**Command 4:** `don't throw exception when buffer overflow, just throw the contents quietly`
- Changed from throwing exceptions to silent truncation

**Command 5:** `don't throw exception at all`
- Removed all exception throwing, including from `at()` method
- Removed `#include <stdexcept>`

**Command 6:** `why we need to implement to_chars? there is already std::to_chars`
- Replaced custom `to_chars` implementation with `std::to_chars`
- Added `#include <charconv>`
- Added `#include <system_error>` for `std::errc`

**Command 7:** `the Deducton guide isn't very useful`
- Removed the template deduction guide

**Command 8:** `change function at line 103: in case room left in buffer is less than 16 bytes, give up`
- Added check: `if (N - m_size < 16) return *this;`

**Command 9:** `what exactly number of chars of uint64_t in decimal`
- Answer: 20 characters maximum

**Command 10:** `then let's change line 104 to use 20`
- Changed from 16 to 20 bytes

**Command 11:** `is there a constant for this number?`
- Created constant: `constexpr std::size_t max_integer_decimal_chars = 20;`

**Command 12:** `let's not define a local buffer, must m_data directly`
- Changed integer append to write directly to `m_data` buffer
- Removed local `char buffer[32]`

**Command 13:** `do we really need the check at line 109? since std::to_chars will check anyway`
- Removed redundant capacity check before calling `std::to_chars`

## Feature Additions

**Command 14:** `let's add a type case operator to const char *`
```cpp
operator const char*() const noexcept
```

**Command 15:** `since we have implicit type cast to const char*, no need to call c_str()`
- Removed all `c_str()` calls from examples

**Command 16:** `now let's add operator << for appending datas`
```cpp
template <typename T>
constexpr StackString& operator<<(T&& value)
```

**Command 17:** `add a constructor with variadic parameters which are data to be appended`
```cpp
template <typename... Args>
constexpr StackString(Args&&... args)
```

**Command 18:** `also add test case to append StackString to std::string`
```cpp
std::string std_str = "Prefix: ";
std_str += stack_str;
```

**Command 19:** `also show append to std::string with operator <<`
- Added example with `std::ostringstream`

## Documentation

**Command 20:** `update README.md`
- Updated features, API reference, and examples

**Command 21:** `let's commit it`
```bash
git init
git add -A
git commit -m "Initial commit: C++17 StackString library with stack-allocated string storage..."
```

**Command 22:** `let's push it to github`

**Command 23:** `let's push to remote repository https://github.com/Anluren/stack_string.git`
```bash
git remote add origin https://github.com/Anluren/stack_string.git
git branch -M main
git push -u origin main
```

**Command 24:** `add document to explain the design and features`
- Created comprehensive `DESIGN.md`

**Command 25:** `move the file to docs directory`
```bash
mkdir -p docs
mv DESIGN.md docs/
git add -A
git commit -m "Add comprehensive design documentation..."
git push
```

## CMake Restructuring

**Command 26:** `create CMakeLists.txt in examples and include to handle files there`
- Created `examples/CMakeLists.txt`
- Main CMakeLists.txt uses `add_subdirectory(examples)`

**Command 27:** `add CMakeLists.txt in directory include as well`
- Created `include/CMakeLists.txt`

**Command 28:** `move add_library for header file to include directory`
- Moved library target definition to `include/CMakeLists.txt`

**Command 29:** `at line 20, check whether this is root of whole workspace, if not, don't build examples`
- Added check: `CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR`

**Command 30:** `write down all the commands I gave you to a markdown file in docs`
- Created this file: `docs/HISTORY.md`

## Summary


## BufferAllocator Development

**Command 32:** `let's add this in to the library`
- Context: User wanted to add BufferAllocator for std::basic_string compatibility
- Created `include/buffer_allocator.hpp` with custom allocator using shared_ptr<State>
- Created `examples/buffer_allocator_usage.cpp` to demonstrate usage
- Updated CMake files to build the new example

**Command 33:** `to make it simpler, don't support reallocation in case buffer is exhausted`
- Simplified BufferAllocator to return nullptr when buffer exhausted
- Removed fallback to heap allocation
- Made allocator purely monotonic

**Command 34:** `why use shared pointer at line 29?`
- Explanation: shared_ptr was used to share state between allocator copies

**Command 35:** `let's do it`
- Context: Remove shared_ptr dependency from BufferAllocator
- Removed `#include <memory>`
- Made State struct public
- Changed from `shared_ptr<State>` to raw `State*` pointer
- User manages State lifetime explicitly
- Updated examples and README

**Command 36:** `let's not use state at all`
- Removed State struct completely
- Allocator now takes buffer pointer, capacity, and usage counter directly as constructor parameters
- Changed from `BufferAllocator(State*)` to `BufferAllocator(void*, size_t, size_t*)`
- Updated examples to pass `&used` counter

**Command 37:** `why the buffer need to have third parameter?`
- Explanation: Usage counter needed to track allocations across multiple allocator instances

**Command 38:** `I see, what if we want to have just one instance of std::string using that buffer?`
- Simplified to remove third parameter
- Allocator tracks usage internally with `m_used` member
- Changed from `BufferAllocator(void*, size_t, size_t*)` to `BufferAllocator(void*, size_t)`
- Designed for single string instance per buffer
- Updated examples and README

**Command 39:** `can you add code to demostrate mixed usage of BufferString and StackString?`
- Created `examples/mixed_usage.cpp` showing interoperability
- Demonstrated conversions between StackString and BufferString
- Showed implicit conversions to const char* and string_view
- Added example to CMakeLists.txt
- Updated README with mixed usage example

**Command 40:** `let's update documentation as well`
- Updated README.md with mixed usage example and BufferAllocator details
- Updated DESIGN.md with complete BufferAllocator architecture section
- Added component interoperability documentation
- Documented design decisions and when to use each component

**Command 41:** `let's use a separate library for buffer_allocator.hpp`
- Split into two separate CMake library targets
- Created `buffer_allocator` library target alongside `stack_string`
- Updated examples to link with appropriate libraries

**Command 42:** `rename buffer_allocator as fixed_buf_allocator`
- Renamed class: `BufferAllocator` → `FixedBufAllocator`
- Renamed file: `buffer_allocator.hpp` → `fixed_buf_allocator.hpp`
- Renamed CMake target: `buffer_allocator` → `fixed_buf_allocator`
- Renamed example executable: `buffer_allocator_usage` → `fixed_buf_allocator_usage`
- Updated all documentation and example code
- Reflects the fixed-size, single-buffer-per-string design

## Summary

The development progressed from a basic stack-allocated string class to a fully-featured, well-documented library with:
- **StackString**: Fixed-capacity template-based string class
- **FixedBufAllocator**: Custom allocator for std::basic_string with fixed external buffer
- Multiple construction and append methods
- Stream-style syntax (`operator<<`)
- Variadic constructor
- Implicit conversions for seamless integration
- No exceptions policy (silent truncation)
- Efficient integer conversion using `std::to_chars`
- Component interoperability via const char* and string_view
- Comprehensive documentation (README, DESIGN, HISTORY)
- Modular CMake structure with separate library targets
- GitHub repository with full version control

