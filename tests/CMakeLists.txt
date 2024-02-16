# Test executable
add_executable(bplustreetest
bplustree.t.cpp
splitnode.t.cpp
)

target_compile_options(bplustreetest PUBLIC -g)

# Link libraries if needed
target_link_libraries(bplustreetest PRIVATE
 bplustree
 gtest
 gtest_main
 pthread)

# Optional: Set the output directories for executables
set_target_properties(bplustreetest
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_test(NAME bplustreetest COMMAND bplustreetest)