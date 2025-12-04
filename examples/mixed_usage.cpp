#include "stack_string.hpp"
#include "fixed_buf_allocator.hpp"
#include <iostream>
#include <string>

using namespace stack_string;

int main() {
    std::cout << "=== Mixed Usage: StackString and BufferString ===" << std::endl;
    std::cout << std::endl;
    
    // Create a StackString
    StackString<128> stack_str;
    stack_str << "StackString: " << 42 << " items";
    
    std::cout << "StackString content: \"" << stack_str.c_str() << "\"" << std::endl;
    std::cout << "  Size: " << stack_str.size() << " / 128" << std::endl;
    std::cout << std::endl;
    
    // Create a BufferString
    char buffer[256];
    FixedBufAllocator<char> alloc(buffer, sizeof(buffer));
    using BufferString = std::basic_string<char, std::char_traits<char>, FixedBufAllocator<char>>;
    
    BufferString buffer_str(alloc);
    buffer_str = "BufferString: " + std::to_string(100) + " records";
    
    std::cout << "BufferString content: \"" << buffer_str << "\"" << std::endl;
    std::cout << "  Size: " << buffer_str.size() << std::endl;
    std::cout << std::endl;
    
    // Convert StackString to BufferString
    StackString<64> stack_msg;
    stack_msg << "Message " << 1 << ": Hello";
    
    BufferString converted_str(alloc);
    converted_str = stack_msg.c_str();  // Uses implicit conversion to const char*
    
    std::cout << "Converted from StackString: \"" << converted_str << "\"" << std::endl;
    std::cout << std::endl;
    
    // Convert BufferString to StackString
    buffer_str = "Status: OK";
    
    StackString<64> stack_from_buffer;
    stack_from_buffer.append(buffer_str.c_str());
    stack_from_buffer << " (verified)";
    
    std::cout << "Converted from BufferString: \"" << stack_from_buffer.c_str() << "\"" << std::endl;
    std::cout << std::endl;
    
    // Use both in combination
    StackString<128> report;
    report << "Report: ";
    report.append(stack_str.c_str());
    report << " | ";
    report.append(buffer_str.c_str());
    
    std::cout << "Combined report: \"" << report.c_str() << "\"" << std::endl;
    std::cout << std::endl;
    
    // Pass StackString to functions expecting const char*
    auto print_message = [](const char* msg) {
        std::cout << "Function received: \"" << msg << "\"" << std::endl;
    };
    
    StackString<64> func_test;
    func_test << "Implicit conversion test";
    print_message(func_test);  // Implicit conversion to const char*
    std::cout << std::endl;
    
    // Use with std::string_view
    StackString<64> view_test;
    view_test << "String view compatible";
    
    std::string_view sv = view_test;  // Implicit conversion to string_view
    std::cout << "As string_view: \"" << sv << "\"" << std::endl;
    std::cout << "  Substring: \"" << sv.substr(0, 11) << "\"" << std::endl;
    std::cout << std::endl;
    
    // Performance comparison scenario
    std::cout << "=== Performance Scenario ===" << std::endl;
    
    // StackString: No heap allocation, fixed capacity
    StackString<256> stack_log;
    for (int i = 0; i < 5; ++i) {
        stack_log << "Event " << i << "; ";
    }
    std::cout << "StackString log: \"" << stack_log.c_str() << "\"" << std::endl;
    
    // BufferString: No heap allocation (if fits in buffer), std::string compatibility
    char log_buffer[256];
    FixedBufAllocator<char> log_alloc(log_buffer, sizeof(log_buffer));
    BufferString buffer_log(log_alloc);
    
    for (int i = 0; i < 5; ++i) {
        buffer_log += "Item " + std::to_string(i) + "; ";
    }
    std::cout << "BufferString log: \"" << buffer_log << "\"" << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== Summary ===" << std::endl;
    std::cout << "StackString: Lightweight, template-based, no heap allocation" << std::endl;
    std::cout << "BufferString: std::string compatible, allocator-based, no heap allocation" << std::endl;
    std::cout << "Both: Interoperable via const char* and string_view conversions" << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== Demo Complete ===" << std::endl;
    
    return 0;
}
