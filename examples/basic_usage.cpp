#include "stack_string.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace stack_string;

int main() {
    // Create a StackString with capacity of 256 characters
    StackString<256> str;
    
    std::cout << "=== StackString Demo ===" << std::endl;
    std::cout << "Initial capacity: " << str.max_size() << std::endl;
    std::cout << "Initial size: " << str.size() << std::endl;
    std::cout << std::endl;

    // Append const char*
    str.append("Hello, ");
    std::cout << "After append(\"Hello, \"): " << str << std::endl;
    std::cout << "Size: " << str.size() << std::endl;

    // Append std::string_view
    std::string_view world = "World";
    str.append(world);
    std::cout << "After append(string_view \"World\"): " << str << std::endl;

    // Append char
    str += '!';
    std::cout << "After append('!'): " << str << std::endl;

    // Clear and start fresh
    str.clear();
    std::cout << "\nAfter clear(): \"" << str << "\"" << std::endl;
    std::cout << "Size: " << str.size() << std::endl;
    
    // Demonstrate integer appending
    str = "The answer is: ";
    str.append(42);
    std::cout << "\nAfter append(42): " << str << std::endl;

    str.clear();
    str = "Numbers: ";
    str += -100;
    str += ", ";
    str += 0;
    str += ", ";
    str += 999;
    std::cout << "After appending multiple integers: " << str << std::endl;

    // Demonstrate different constructors
    StackString<64> str2("Constructed from literal");
    std::cout << "\nConstructed from literal: " << str2 << std::endl;

    StackString<64> str3(std::string_view("From string_view"));
    std::cout << "Constructed from string_view: " << str3 << std::endl;

    // Copy constructor
    StackString<64> str4(str2);
    std::cout << "Copy constructed: " << str4 << std::endl;

    // Conversion to string_view
    std::string_view sv = str2;
    std::cout << "Converted to string_view: " << sv << std::endl;

    // Demonstrate chaining
    StackString<128> path;
    path.append("/home/").append("user").append("/").append("documents");
    std::cout << "\nChained appends: " << path << std::endl;

    // Building a message with mixed types
    StackString<256> msg;
    msg = "User ";
    msg += 1001;
    msg += " has ";
    msg += 5;
    msg += " new messages";
    std::cout << "Mixed type message: " << msg << std::endl;

    // Demonstrate operator<< for chaining
    StackString<128> stream_style;
    stream_style << "Value: " << 123 << ", Name: " << "Test";
    std::cout << "Stream style: " << stream_style << std::endl;

    // Demonstrate variadic constructor
    StackString<128> variadic("ID: ", 42, ", Status: ", "Active");
    std::cout << "Variadic constructor: " << variadic << std::endl;

    // Demonstrate appending StackString to std::string
    std::string std_str = "Prefix: ";
    StackString<64> stack_str("StackString content");
    std_str += stack_str;  // Implicit conversion to const char*
    std::cout << "StackString appended to std::string: " << std_str << std::endl;

    // Also works with std::ostringstream
    std::ostringstream oss;
    oss << "Stream output: " << stack_str << " (via operator<<)";
    std::cout << oss.str() << std::endl;

    // Demonstrate element access
    std::cout << "\nElement access:" << std::endl;
    std::cout << "First character: " << msg[0] << std::endl;
    std::cout << "Last character: " << msg[msg.size() - 1] << std::endl;

    // Demonstrate iteration
    StackString<32> iter_test = "ABC123";
    std::cout << "\nIterating over \"" << iter_test << "\": ";
    for (char c : iter_test) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;

    // Demonstrate available space
    StackString<32> small;
    small = "Hello";
    std::cout << "\nSmall string capacity: " << small.max_size() << std::endl;
    std::cout << "Current size: " << small.size() << std::endl;
    std::cout << "Available space: " << small.available() << std::endl;

    // Comparison operators
    StackString<32> comp1 = "test";
    StackString<32> comp2 = "test";
    StackString<32> comp3 = "different";
    
    std::cout << "\nComparisons:" << std::endl;
    std::cout << "\"test\" == \"test\": " << (comp1 == comp2 ? "true" : "false") << std::endl;
    std::cout << "\"test\" == \"different\": " << (comp1 == comp3 ? "true" : "false") << std::endl;
    std::cout << "\"test\" == string_view: " << (comp1 == std::string_view("test") ? "true" : "false") << std::endl;

    // Error handling demonstration
    std::cout << "\n=== Error Handling ===" << std::endl;
    try {
        StackString<10> tiny;
        tiny = "Short";
        std::cout << "Tiny string: " << tiny << std::endl;
        
        // This will throw because it exceeds capacity
        tiny.append(" - this is way too long for the capacity");
    } catch (const std::length_error& e) {
        std::cout << "Caught expected error: " << e.what() << std::endl;
    }

    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}
