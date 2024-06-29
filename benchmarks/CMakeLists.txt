add_executable(bm
    benchmark.cpp
)

target_link_libraries(bm PUBLIC benchmark::benchmark bplustree)