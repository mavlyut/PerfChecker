#include <benchmark/benchmark.h>

#include <cstdint>
#include <limits>
#include <random>
#include <vector>

std::vector<uint64_t> GenRandom(size_t size) {
  std::vector<uint64_t> result;
  result.resize(size);

  std::default_random_engine engine(42);
  std::uniform_int_distribution<uint64_t> randomElement(
      0, std::numeric_limits<uint64_t>::max());
  for (auto& e : result) {
    e = randomElement(engine);
  }

  return result;
}

size_t CountZeroBit(const std::vector<uint64_t>& input, int bit) {
  size_t count = 0;
  for (auto e : input) {
    if ((e >> bit) & 1) {
      count++;
    }
  }
  return input.size() - count;
}

// DO NOT change the algorithm.
void RadixSort(std::vector<uint64_t>* input) {
  for (int bit = 0; bit < 64; ++bit) {
    auto count = CountZeroBit(*input, bit);

    std::vector<uint64_t> buffer;
    buffer.resize(input->size());

    size_t i = 0, j = count;
    for (auto e : *input) {
      if ((e >> bit) & 1) {
        buffer[j++] = e;
      } else {
        buffer[i++] = e;
      }
    }

    input->swap(buffer);
  }
}

void BM_RadixSort(benchmark::State& state) {
  auto input = GenRandom(16 << 20);
  auto copy = input;

  for (auto _ : state) {
    copy = input;
    RadixSort(&copy);

    benchmark::DoNotOptimize(copy);
  }

  if (!std::is_sorted(copy.begin(), copy.end())) {
    state.SkipWithError("Sort is not sorting");
  }
}

BENCHMARK(BM_RadixSort)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
