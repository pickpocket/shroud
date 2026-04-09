# Crackme: simple password checker to demonstrate obfuscation
add_executable(crackme crackme.c)

# Also build an "obfuscated" version using the core library
# This applies MBA and opaque predicates at the C++ level
add_executable(crackme_obfuscated crackme_obfuscated.cpp)
target_link_libraries(crackme_obfuscated PRIVATE shroud_core)
