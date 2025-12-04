# StackString

A C++17 header-only library providing efficient stack-based string storage without heap allocations.

## Components

### 1. StackString
A `std::string`-like structure that uses pre-allocated stack memory for fixed-capacity string operations.

### 2. FixedBufAllocator
A custom allocator for `std::basic_string` that uses an external fixed buffer, enabling stack-based allocation with standard library compatibility. Designed for one string instance per buffer.

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

### Using FixedBufAllocator with std::string

```cpp
#include <fixed_buf_allocator.hpp>
#include <string>

using namespace stack_string;

int main() {
    // Create stack buffer
    char buffer[256];
    
    // Create allocator
    FixedBufAllocator<char> alloc(buffer, sizeof(buffer));
    
    // Use with std::basic_string (one string per buffer)
    using BufferString = std::basic_string<char, std::char_traits<char>, 
                                           FixedBufAllocator<char>>;
    
    BufferString str(alloc);
    str = "Uses stack memory!";
    str += " Very efficient!";
    
    return 0;
}
```

### Building with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .

# Run the examples
./examples/basic_usage
./examples/fixed_buf_allocator_usage
./examples/mixed_usage
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

### Mixed Usage of StackString and BufferString

```cpp
#include <stack_string.hpp>
#include <fixed_buf_allocator.hpp>

using namespace stack_string;

// StackString: Lightweight, template-based
StackString<128> stack_str;
stack_str << "Count: " << 42;

// BufferString: std::string compatible, allocator-based
char buffer[256];
FixedBufAllocator<char> alloc(buffer, sizeof(buffer));
using BufferString = std::basic_string<char, std::char_traits<char>, 
                                       FixedBufAllocator<char>>;
BufferString buffer_str(alloc);
buffer_str = "Total: " + std::to_string(100);

// Convert between them using implicit const char* conversion
BufferString from_stack(alloc);
from_stack = stack_str.c_str();

StackString<128> from_buffer;
from_buffer.append(buffer_str.c_str());

// Both work with string_view
std::string_view sv1 = stack_str;
std::string_view sv2 = buffer_str;
```

See `examples/mixed_usage.cpp` for a complete demonstration.

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
