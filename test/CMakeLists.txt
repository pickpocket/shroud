include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
# For Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# Opaque Predicates tests
add_executable(OpaquePredicatesTest OpaquePredicatesTest.cpp)
target_link_libraries(OpaquePredicatesTest PRIVATE shroud_core GTest::gtest_main)
gtest_discover_tests(OpaquePredicatesTest)

# MBA tests
add_executable(MBATest MBATest.cpp)
target_link_libraries(MBATest PRIVATE shroud_core GTest::gtest_main)
gtest_discover_tests(MBATest)

# Overlapping Instructions tests
add_executable(OverlappingInstructionsTest OverlappingInstructionsTest.cpp)
target_link_libraries(OverlappingInstructionsTest PRIVATE shroud_core GTest::gtest_main)
gtest_discover_tests(OverlappingInstructionsTest)

# Integration tests
add_executable(IntegrationTest IntegrationTest.cpp)
target_link_libraries(IntegrationTest PRIVATE shroud_core GTest::gtest_main)
gtest_discover_tests(IntegrationTest)
