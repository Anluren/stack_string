#pragma once

#include <cstddef>
#include <cstring>
#include <string_view>
#include <type_traits>
#include <algorithm>
#include <charconv>
#include <system_error>

namespace stack_string {

// Maximum number of characters needed to represent any integer in decimal
// (20 digits for uint64_t max value: 18,446,744,073,709,551,615)
constexpr std::size_t max_integer_decimal_chars = 20;

/**
 * A fixed-capacity string that uses stack-allocated memory.
 * Similar to std::string but with pre-allocated storage on the stack.
 * 
 * @tparam N Maximum capacity (excluding null terminator)
 */
template <std::size_t N>
class StackString {
public:
    static constexpr std::size_t capacity = N;

    // Constructors
    constexpr StackString() noexcept : m_size(0) {
        m_data[0] = '\0';
    }

    constexpr StackString(const char* str) : m_size(0) {
        append(str);
    }

    constexpr StackString(std::string_view sv) : m_size(0) {
        append(sv);
    }

    template <typename... Args,
              typename = std::enable_if_t<(sizeof...(Args) > 1)>>
    constexpr StackString(Args&&... args) : m_size(0) {
        m_data[0] = '\0';
        (append(std::forward<Args>(args)), ...);
    }

    // Copy and move constructors
    constexpr StackString(const StackString& other) noexcept : m_size(other.m_size) {
        std::copy(other.m_data, other.m_data + m_size + 1, m_data);
    }

    constexpr StackString& operator=(const StackString& other) noexcept {
        if (this != &other) {
            m_size = other.m_size;
            std::copy(other.m_data, other.m_data + m_size + 1, m_data);
        }
        return *this;
    }

    constexpr StackString(StackString&& other) noexcept : m_size(other.m_size) {
        std::copy(other.m_data, other.m_data + m_size + 1, m_data);
        other.clear();
    }

    constexpr StackString& operator=(StackString&& other) noexcept {
        if (this != &other) {
            m_size = other.m_size;
            std::copy(other.m_data, other.m_data + m_size + 1, m_data);
            other.clear();
        }
        return *this;
    }

    // Append operations
    constexpr StackString& append(const char* str) {
        if (!str) return *this;
        
        std::size_t len = std::strlen(str);
        std::size_t space_available = N - m_size;
        std::size_t to_copy = (len > space_available) ? space_available : len;
        
        std::copy(str, str + to_copy, m_data + m_size);
        m_size += to_copy;
        m_data[m_size] = '\0';
        
        return *this;
    }

    constexpr StackString& append(std::string_view sv) {
        std::size_t space_available = N - m_size;
        std::size_t to_copy = (sv.size() > space_available) ? space_available : sv.size();
        
        std::copy(sv.begin(), sv.begin() + to_copy, m_data + m_size);
        m_size += to_copy;
        m_data[m_size] = '\0';
        
        return *this;
    }

    constexpr StackString& append(char c) {
        if (m_size >= N) {
            return *this;
        }
        
        m_data[m_size++] = c;
        m_data[m_size] = '\0';
        
        return *this;
    }

    // Append integer types
    template <typename T>
    constexpr std::enable_if_t<std::is_integral_v<T>, StackString&>
    append(T value) {
        auto [ptr, ec] = std::to_chars(m_data + m_size, m_data + N, value);
        if (ec == std::errc()) {
            m_size = ptr - m_data;
            m_data[m_size] = '\0';
        }
        return *this;
    }

    // Operator overloads
    constexpr StackString& operator+=(const char* str) {
        return append(str);
    }

    constexpr StackString& operator+=(std::string_view sv) {
        return append(sv);
    }

    constexpr StackString& operator+=(char c) {
        return append(c);
    }

    template <typename T>
    constexpr std::enable_if_t<std::is_integral_v<T>, StackString&>
    operator+=(T value) {
        return append(value);
    }

    template <typename T>
    constexpr StackString& operator<<(T&& value) {
        return append(std::forward<T>(value));
    }

    // Access operations
    constexpr const char* c_str() const noexcept {
        return m_data;
    }

    constexpr const char* data() const noexcept {
        return m_data;
    }

    constexpr char* data() noexcept {
        return m_data;
    }

    constexpr std::size_t size() const noexcept {
        return m_size;
    }

    constexpr std::size_t length() const noexcept {
        return m_size;
    }

    constexpr bool empty() const noexcept {
        return m_size == 0;
    }

    constexpr std::size_t max_size() const noexcept {
        return N;
    }

    constexpr std::size_t available() const noexcept {
        return N - m_size;
    }

    constexpr operator std::string_view() const noexcept {
        return std::string_view(m_data, m_size);
    }

    constexpr operator const char*() const noexcept {
        return m_data;
    }

    // Element access
    constexpr char& operator[](std::size_t pos) noexcept {
        return m_data[pos];
    }

    constexpr const char& operator[](std::size_t pos) const noexcept {
        return m_data[pos];
    }

    constexpr char& at(std::size_t pos) {
        return m_data[pos];
    }

    constexpr const char& at(std::size_t pos) const {
        return m_data[pos];
    }

    // Iterators
    constexpr char* begin() noexcept { return m_data; }
    constexpr const char* begin() const noexcept { return m_data; }
    constexpr const char* cbegin() const noexcept { return m_data; }
    
    constexpr char* end() noexcept { return m_data + m_size; }
    constexpr const char* end() const noexcept { return m_data + m_size; }
    constexpr const char* cend() const noexcept { return m_data + m_size; }

    // Modifiers
    constexpr void clear() noexcept {
        m_size = 0;
        m_data[0] = '\0';
    }

    constexpr void resize(std::size_t count, char ch = '\0') {
        if (count > N) {
            count = N;
        }
        
        if (count > m_size) {
            std::fill(m_data + m_size, m_data + count, ch);
        }
        
        m_size = count;
        m_data[m_size] = '\0';
    }

    // Comparison operators
    constexpr bool operator==(const StackString& other) const noexcept {
        return std::string_view(*this) == std::string_view(other);
    }

    constexpr bool operator!=(const StackString& other) const noexcept {
        return !(*this == other);
    }

    constexpr bool operator==(std::string_view sv) const noexcept {
        return std::string_view(*this) == sv;
    }

    constexpr bool operator!=(std::string_view sv) const noexcept {
        return !(*this == sv);
    }

    constexpr bool operator==(const char* str) const noexcept {
        return std::string_view(*this) == str;
    }

    constexpr bool operator!=(const char* str) const noexcept {
        return !(*this == str);
    }

private:
    char m_data[N + 1]; // +1 for null terminator
    std::size_t m_size;
};

} // namespace stack_string
