# LLVM Pass Plugin — requires LLVM dev headers and libraries
# Build with: cmake .. -DBUILD_LLVM_PLUGIN=ON -DLLVM_DIR=<path>/lib/cmake/llvm

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_library(ShroudPasses MODULE
    PassPlugin.cpp
    OpaquePredicatesPass.cpp
    MBAPass.cpp
    OverlappingInstructionsPass.cpp
    StringHidePass.cpp
)

# Runtime library set via CMAKE_MSVC_RUNTIME_LIBRARY from parent

# Fix DIA SDK path (LLVM cmake config may reference the wrong VS edition)
find_path(DIA_SDK_LIB_DIR diaguids.lib
    PATHS
    "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/DIA SDK/lib/amd64"
    "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/DIA SDK/lib/amd64"
    "C:/Program Files/Microsoft Visual Studio/2022/Community/DIA SDK/lib/amd64"
)
if(DIA_SDK_LIB_DIR)
    link_directories(${DIA_SDK_LIB_DIR})
endif()

# Get the LLVM libraries needed for a pass plugin
llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    TransformUtils
    Passes
    TargetParser
)

target_link_libraries(ShroudPasses PRIVATE shroud_core ${LLVM_LIBS})
target_include_directories(ShroudPasses PRIVATE ${LLVM_INCLUDE_DIRS})

# Plugin output settings
set_target_properties(ShroudPasses PROPERTIES
    PREFIX ""
    SUFFIX ".dll"
)

if(NOT WIN32)
    set_target_properties(ShroudPasses PROPERTIES
        LINK_FLAGS "-Wl,-undefined,dynamic_lookup"
    )
endif()
