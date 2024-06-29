#include <benchmark/benchmark.h>
#include "page.h" 

// Benchmark insert operation
static void BM_Insert(benchmark::State& state) {
    auto memory = malloc(100 * 1024);
    Page<int, int> page(memory, 100 * 1024, PageType::KEY_VALUE);
    for (auto _ : state) {
        page.insertData(rand(), rand()); // Replace rand() with your data generation logic
        page.find(rand());
    }
}
BENCHMARK(BM_Insert);


// Main function to run the benchmarks
BENCHMARK_MAIN();
