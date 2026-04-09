cmake_minimum_required(VERSION 3.20)
project(shroud VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Core obfuscator library (no LLVM dependency)
add_subdirectory(lib)

# Tests
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# Crackme
add_subdirectory(crackme)

# LLVM pass plugin (optional, requires LLVM dev headers)
option(BUILD_LLVM_PLUGIN "Build LLVM pass plugin (requires LLVM dev headers)" OFF)
if(BUILD_LLVM_PLUGIN)
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
    add_subdirectory(pass)
endif()
