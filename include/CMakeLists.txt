# StackString header-only library

# Header-only library target
add_library(stack_string INTERFACE)
target_include_directories(stack_string INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Install headers
install(FILES stack_string.hpp
    DESTINATION include
)
