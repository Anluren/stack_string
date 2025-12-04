#include "fixed_buf_allocator.hpp"
#include <iostream>
#include <string>

using namespace stack_string;

int main() {
    std::cout << "=== FixedBufAllocator Demo ===" << std::endl;
    
    // Create a stack buffer
    char buffer[256];
    
    // Create allocator with buffer
    FixedBufAllocator<char> alloc(buffer, sizeof(buffer));
    
    // Create std::string using the custom allocator
    using BufferString = std::basic_string<char, std::char_traits<char>, FixedBufAllocator<char>>;
    
    BufferString str(alloc);
    
    std::cout << "Initial capacity: " << sizeof(buffer) << " bytes" << std::endl;
    std::cout << std::endl;
    
    // Append some data
    str = "Hello, BufferAllocator!";
    std::cout << "After assignment: \"" << str << "\"" << std::endl;
    std::cout << "  String size: " << str.size() << std::endl;
    std::cout << std::endl;
    
    // Append more data
    str += " This uses stack memory.";
    std::cout << "After append: \"" << str << "\"" << std::endl;
    std::cout << "  String size: " << str.size() << std::endl;
    std::cout << std::endl;
    
    // Append more
    str += " Very efficient!";
    std::cout << "After another append: \"" << str << "\"" << std::endl;
    std::cout << "  String size: " << str.size() << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== Demo Complete ===" << std::endl;
    
    return 0;
}
