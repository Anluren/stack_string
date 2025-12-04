# StackString header-only library

# Header-only library target for StackString
add_library(stack_string INTERFACE)
target_include_directories(stack_string INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Header-only library target for FixedBufAllocator
add_library(fixed_buf_allocator INTERFACE)
target_include_directories(fixed_buf_allocator INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Install headers
install(FILES 
    stack_string.hpp
    DESTINATION include
)

install(FILES 
    fixed_buf_allocator.hpp
    DESTINATION include
)
