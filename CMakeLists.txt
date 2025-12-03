cmake_minimum_required(VERSION 3.10)
project(StackString VERSION 1.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Add include subdirectory (defines stack_string target)
add_subdirectory(include)

# Examples
option(BUILD_EXAMPLES "Build example programs" ON)

if(BUILD_EXAMPLES AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(examples)
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS stack_string
    EXPORT StackStringTargets
)

# Installation handled in include/CMakeLists.txt

install(EXPORT StackStringTargets
    FILE StackStringTargets.cmake
    NAMESPACE StackString::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StackString
)

# Create package config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/StackStringConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/StackStringConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StackString
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/StackStringConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/StackStringConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/StackStringConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StackString
)
