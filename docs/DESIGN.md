# StackString Design Document

## Overview

StackString is a C++17 header-only library that provides a fixed-capacity string class with stack-allocated storage. It offers a `std::string`-like interface while avoiding heap allocations, making it ideal for performance-critical applications, embedded systems, and scenarios where memory allocation overhead must be minimized.

## Design Goals

1. **Zero heap allocations**: All storage is on the stack, determined at compile-time
2. **Familiar API**: Closely mimics `std::string` interface for ease of use
3. **Type flexibility**: Support multiple data types (strings, integers, characters)
4. **No exceptions**: Predictable behavior with silent truncation on overflow
5. **Standard library integration**: Seamless interoperability with `std::string`, streams, and algorithms
6. **Compile-time safety**: Capacity known at compile-time via template parameter

## Architecture

### Memory Layout

```cpp
template <std::size_t N>
class StackString {
private:
    char m_data[N + 1];     // +1 for null terminator
    std::size_t m_size;     // Current string length
};
```

- **Fixed array**: Character data stored in a fixed-size array on the stack
- **Null-terminated**: Always maintains null terminator for C compatibility
- **Size tracking**: Separate size member for O(1) length queries
- **Capacity**: Template parameter `N` determines maximum capacity (excluding null terminator)

### Member Naming Convention

- `m_` prefix for all member variables
- `m_data`: The character storage array
- `m_size`: Current string length (not including null terminator)

## Key Features

### 1. Multiple Construction Methods

```cpp
StackString<64> s1;                           // Empty string
StackString<64> s2("Hello");                  // From C string
StackString<64> s3(std::string_view("Hi"));   // From string_view
StackString<64> s4("ID: ", 42, ", OK");       // Variadic (2+ args)
```

**Design rationale**: Provide flexibility for different initialization scenarios. The variadic constructor requires 2+ arguments to avoid ambiguity with copy/move constructors.

### 2. Stream-Style Building

```cpp
StackString<128> msg;
msg << "User " << 1001 << " has " << 5 << " messages";
```

**Design rationale**: Familiar iostream-like syntax for building strings. Implemented via `operator<<` that returns `*this` for chaining.

### 3. Type-Safe Integer Conversion

```cpp
str += 42;        // Appends "42"
str += -100;      // Appends "-100"
```

**Implementation**: Uses `std::to_chars` from `<charconv>` for efficient, locale-independent integer-to-string conversion. Writes directly to `m_data` buffer without temporary allocations.

### 4. Implicit Conversions

```cpp
StackString<64> str("test");
const char* cstr = str;           // Implicit conversion
std::string_view sv = str;        // Implicit conversion
std::cout << str;                 // Works via const char* conversion
```

**Design rationale**: Seamless integration with existing C and C++ string APIs. Two conversion operators:
- `operator const char*()`: For C-style APIs and output streams
- `operator std::string_view()`: For modern C++ APIs

### 5. No-Exception Policy

```cpp
StackString<10> tiny;
tiny = "This is way too long";  // Silently truncates to 10 chars
```

**Design rationale**: 
- Suitable for exception-free environments (embedded systems, real-time applications)
- Predictable behavior without performance overhead of exception handling
- All overflow conditions handled by silent truncation
- `std::to_chars` error codes are checked but errors are silently ignored

### 6. Direct Buffer Writing

For integer conversion, `std::to_chars` writes directly into `m_data`:

```cpp
auto [ptr, ec] = std::to_chars(m_data + m_size, m_data + N, value);
if (ec == std::errc()) {
    m_size = ptr - m_data;
    m_data[m_size] = '\0';
}
```

**Benefits**:
- No temporary buffer allocation
- Single-pass conversion
- Efficient use of available space

## Performance Characteristics

### Time Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Construction | O(n) | Where n is input string length |
| `append(const char*)` | O(n) | Where n is append string length |
| `append(string_view)` | O(n) | Where n is append string length |
| `append(char)` | O(1) | Single character |
| `append(integer)` | O(log n) | Where n is integer value |
| `size()`, `length()` | O(1) | Cached in `m_size` |
| `c_str()`, `data()` | O(1) | Direct pointer return |
| `operator[]`, `at()` | O(1) | Direct array access |
| `clear()` | O(1) | Just sets size to 0 |

### Space Complexity

- **Stack usage**: `sizeof(char) * (N + 1) + sizeof(std::size_t)`
- **Typical overhead**: ~8 bytes (size member) + N + 1 bytes for data
- **No heap allocations**: Ever

### Comparison with std::string

| Feature | StackString | std::string |
|---------|-------------|-------------|
| Storage | Stack | Heap (usually) |
| Capacity | Fixed at compile-time | Dynamic |
| Allocations | 0 | Multiple possible |
| Small String Optimization | N/A | Usually 15-23 bytes |
| Cache locality | Excellent | Good (for small strings) |
| Reallocations | Never | Possible on growth |

## Design Decisions

### 1. Template Parameter for Capacity

**Decision**: Use non-type template parameter `std::size_t N`

**Rationale**:
- Compile-time capacity checking
- Different capacities are different types (type safety)
- Enables optimizations based on known size
- No runtime overhead for capacity storage

**Trade-off**: Cannot change capacity at runtime, but this is intentional for stack allocation.

### 2. Always Null-Terminated

**Decision**: Maintain null terminator at `m_data[m_size]`

**Rationale**:
- C API compatibility without extra work
- `c_str()` and `data()` can return direct pointer (O(1))
- Minimal overhead (1 byte per instance)

### 3. Silent Truncation vs Exceptions

**Decision**: Silently truncate on overflow instead of throwing

**Rationale**:
- Exception-free environments (embedded, real-time)
- Predictable performance (no exception overhead)
- Simpler error handling model
- Common pattern in fixed-size buffer handling

**Alternative considered**: Return `bool` from append operations to indicate success/failure. Rejected because it would break chaining patterns.

### 4. Member Prefix Convention

**Decision**: Use `m_` prefix for member variables

**Rationale**:
- Clear distinction between members and local variables
- Avoids naming conflicts with methods
- Common C++ convention
- Better code readability

### 5. Constexpr Support

**Decision**: Mark many methods as `constexpr`

**Rationale**:
- Enable compile-time string building where possible
- Future-proof for C++20+ constexpr improvements
- No runtime cost for the qualifier

**Limitation**: `std::to_chars` is not constexpr in C++17, so integer append cannot be constexpr.

## Integration Patterns

### With std::string

```cpp
StackString<128> stack_str("data");
std::string std_str = "prefix: ";
std_str += stack_str;  // Implicit const char* conversion
```

### With Streams

```cpp
StackString<64> str("output");
std::cout << str;                    // Output stream
std::ostringstream oss;
oss << "Value: " << str;             // String stream
```

### With Algorithms

```cpp
StackString<32> str = "hello";
std::transform(str.begin(), str.end(), str.begin(), ::toupper);
// str is now "HELLO"
```

### With String View

```cpp
StackString<64> str("test");
std::string_view sv = str;           // Implicit conversion
bool matches = (sv == "test");       // Works seamlessly
```

## Limitations

1. **Fixed capacity**: Cannot grow beyond compile-time limit
2. **No SSO threshold**: Unlike `std::string`, always uses fixed size regardless of content
3. **Type system**: Different capacities are incompatible types
4. **Integer conversion**: Only supports standard integral types, not floating-point
5. **No format specifiers**: Integer conversion is always base-10, no hex/octal support
6. **Not constexpr-complete**: Integer append uses `std::to_chars` which isn't constexpr in C++17

## Use Cases

### Ideal For:

- **Embedded systems**: No heap allocation, predictable memory usage
- **Real-time applications**: No allocation overhead, no exceptions
- **Performance-critical code**: Cache-friendly, minimal overhead
- **Log messages**: Fixed-size buffers, multiple data types
- **Network protocols**: Fixed-size message building
- **Configuration strings**: Known maximum lengths

### Not Ideal For:

- **Unbounded input**: Use `std::string` when capacity unknown
- **Highly dynamic content**: When reallocation flexibility needed
- **Large strings**: Stack space is limited
- **Heavy string manipulation**: `std::string` has more specialized methods

## Future Enhancements

Potential improvements for future versions:

1. **Format support**: `printf`-style formatting
2. **Floating-point**: Support for `float` and `double` conversion
3. **Custom bases**: Hex, octal, binary integer output
4. **String operations**: `substr`, `find`, `replace`, etc.
5. **Comparison operators**: Full set of relational operators
6. **Capacity query**: Constexpr methods to query capacity
7. **Truncation callback**: Optional notification when truncation occurs
8. **C++20 constexpr**: More operations at compile-time

## Conclusion

StackString provides a focused solution for fixed-capacity string building without heap allocations. Its design prioritizes:
- **Performance**: Zero allocations, minimal overhead
- **Safety**: Compile-time capacity, no buffer overflows
- **Usability**: Familiar API, standard library integration
- **Predictability**: No exceptions, deterministic behavior

The library fills a niche between raw character arrays and `std::string`, offering modern C++ conveniences with embedded-friendly constraints.
