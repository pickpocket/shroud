# Core obfuscator library — pure C++, no LLVM dependency
add_library(shroud_core STATIC
    Utils.cpp
    OpaquePredicates.cpp
    MBA.cpp
    OverlappingInstructions.cpp
)

target_include_directories(shroud_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(shroud_core PUBLIC cxx_std_17)

# Force consistent runtime library
# Runtime library set via CMAKE_MSVC_RUNTIME_LIBRARY from parent

# Warnings
if(MSVC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC"))
    target_compile_options(shroud_core PRIVATE /W4)
else()
    target_compile_options(shroud_core PRIVATE -Wall -Wextra)
endif()
