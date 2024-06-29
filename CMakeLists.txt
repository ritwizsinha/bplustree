cmake_minimum_required(VERSION 3.16)
SET(bplustree bplustree)
project(${bplustree} LANGUAGES CXX)
find_package(benchmark REQUIRED)
# Set C++ standard to C++20
set(CMAKE_CXX_COMPILER g++-10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra)
set(CMAKE_BUILD_TYPE Debug)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cc")
# Main executable
add_library(${bplustree} ${SOURCES})

target_compile_options(${bplustree} PUBLIC -g)
target_compile_options(${bplustree} PRIVATE -Wpointer-arith)
# Include directories
target_include_directories(${bplustree} PUBLIC headers)
# Optional: Add testing support
enable_testing()

add_subdirectory(tests)
add_subdirectory(benchmarks)
