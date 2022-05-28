#include <benchmark/benchmark.h>

#include <cstdint>
#include <cstdlib>
#include <random>
#include <ctime>

// Computes PI using monte carlo method.
// DO NOT change the algorithm.
void BM_ComputePi(benchmark::State& state) {
  int64_t total = 0;
  int64_t inside_circle = 0;

  srand(time(NULL));

  for (auto _ : state) {
    int x = rand() % 1000;
    int y = rand() % 1000;

    total++;
    if (x * x + y * y < 1000 * 1000) {
      inside_circle++;
    }
  }

  state.counters["pi"] = 4 * double(inside_circle) / total;
}

BENCHMARK(BM_ComputePi)->Threads(1);

BENCHMARK_MAIN();
