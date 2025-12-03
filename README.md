# StackString

A C++17 header-only library providing a `std::string`-like structure that uses pre-allocated stack memory for efficient string operations without heap allocations.

## Features

- **Stack-allocated**: No heap allocations, all memory is on the stack
- **Fixed capacity**: Template parameter determines maximum string size at compile-time
- **std::string-like API**: Familiar interface with `append()`, `operator+=`, `operator<<`, etc.
- **Multiple append types**: Support for `const char*`, `std::string_view`, `char`, and integer types
- **Implicit conversions**: Automatic conversion to `const char*` and `std::string_view`
- **Stream-style syntax**: Chain operations with `operator<<` for intuitive building
- **Variadic constructor**: Construct and append multiple values in one line
- **Constexpr support**: Many operations can be evaluated at compile-time
- **Iterator support**: Compatible with range-based for loops and STL algorithms
- **No exceptions**: Silently truncates when capacity is exceeded

## Quick Start

### Basic Usage

```cpp
#include <stack_string.hpp>

using namespace stack_string;

int main() {
    // Create a StackString with capacity of 256 characters
    StackString<256> str;
    
    // Append different types
    str.append("Hello, ");
    str += "World";
    str += '!';
    
    std::cout << str << std::endl;  // Output: Hello, World!
    
    // Stream-style building
    StackString<128> msg;
    msg << "User " << 42 << " has " << 5 << " messages";
    std::cout << msg << std::endl;  // Output: User 42 has 5 messages
    
    // Variadic constructor
    StackString<128> multi("ID: ", 1001, ", Status: ", "Active");
    std::cout << multi << std::endl;  // Output: ID: 1001, Status: Active
    
    // Works with std::string and streams
    std::string std_str = "Prefix: ";
    std_str += str;  // Implicit conversion to const char*
    
    return 0;
}
```

### Building with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .

# Run the example
./basic_usage
```

### Integration

#### As a subdirectory

```cmake
add_subdirectory(stack_string)
target_link_libraries(your_target PRIVATE stack_string)
```

#### After installation

```cmake
find_package(StackString REQUIRED)
target_link_libraries(your_target PRIVATE StackString::stack_string)
```

## API Reference

### Constructor

```cpp
StackString<N>()                              // Default constructor (empty string)
StackString<N>(const char* str)               // Construct from C string
StackString<N>(std::string_view sv)           // Construct from string_view
StackString<N>(Args&&... args)                // Variadic constructor (2+ args)
```

### Append Operations

```cpp
append(const char* str)                       // Append C string
append(std::string_view sv)                   // Append string_view
append(char c)                                // Append single character
append(T integer)                             // Append integer (any integral type)

operator+=(...)                               // Same as append
operator<<(...)                               // Stream-style append (chainable)
```

### Access

```cpp
c_str()                                       // Get null-terminated C string
data()                                        // Get raw data pointer
size(), length()                              // Current string length
empty()                                       // Check if empty
max_size()                                    // Maximum capacity
available()                                   // Remaining capacity
operator[](pos)                               // Element access (no bounds check)
at(pos)                                       // Element access (same as operator[])
```

### Conversion

```cpp
operator std::string_view()                   // Implicit conversion to string_view
operator const char*()                        // Implicit conversion to C string
```

### Modifiers

```cpp
clear()                             // Clear the string
resize(count, ch = '\0')            // Resize string
```

### Iterators

```cpp
begin(), end()                      // Mutable iterators
cbegin(), cend()                    // Const iterators
```

## Examples

### Building a path

```cpp
StackString<256> path;
path.append("/home/").append("user").append("/").append("documents");
```

### Stream-style concatenation

```cpp
StackString<128> msg;
msg << "User " << 1001 << " has " << 5 << " new messages";
// Result: "User 1001 has 5 new messages"
```

### Variadic constructor

```cpp
StackString<128> log("Level: ", "INFO", ", Code: ", 200);
// Result: "Level: INFO, Code: 200"
```

### Integration with std::string

```cpp
StackString<64> stack_str("content");
std::string std_str = "Prefix: ";
std_str += stack_str;  // Implicit conversion
// Result: "Prefix: content"

std::ostringstream oss;
oss << "Output: " << stack_str;  // Works with streams
```

### Iteration

```cpp
StackString<32> str = "Hello";
for (char c : str) {
    std::cout << c << ' ';
}
```

## Requirements

- C++17 or later
- CMake 3.10 or later (for building examples)

## License

This library is provided as-is for educational and commercial use.

## Design Notes

- The library uses a fixed-size character array on the stack, making it ideal for performance-critical code where allocation overhead must be avoided
- Capacity is determined at compile-time via template parameter
- Integer-to-string conversion uses `std::to_chars` and writes directly to the buffer
- All operations maintain null-termination for C string compatibility
- **No exceptions**: Operations silently truncate content when capacity is exceeded
- Implicit conversions to `const char*` and `std::string_view` enable seamless integration with standard library
