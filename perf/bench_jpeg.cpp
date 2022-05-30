#include <benchmark/benchmark.h>

#include <random>
#include <vector>

typedef std::array<int, 3> Pixel;

typedef std::vector<std::vector<Pixel>> Picture;

Picture MakeRandom() {
  const size_t HEIGHT = 600, WIDTH = 800;

  std::default_random_engine engine(42);
  std::uniform_int_distribution<int> randomPixel(0, 255);

  Picture picture;
  for (size_t i = 0; i < HEIGHT; i++) {
    picture.emplace_back();
    for (size_t j = 0; j < WIDTH; j++) {
      picture.back().emplace_back() =
          Pixel{randomPixel(engine), randomPixel(engine), randomPixel(engine)};
    }
  }
  return picture;
}

Picture ScaleDown(const Picture& picture) {
  Picture scaled;

  for (size_t i = 0; i < picture.size(); i += 2) {
    scaled.emplace_back();
    for (size_t j = 0; j < picture[0].size(); j += 2) {
      auto avgComponent = [&](size_t componentIndex) {
        return (picture[i][j][componentIndex] +
                picture[i + 1][j][componentIndex] +
                picture[i][j + 1][componentIndex] +
                picture[i + 1][j + 1][componentIndex]) /
               4;
      };

      scaled.back().emplace_back() =
          Pixel{avgComponent(0), avgComponent(1), avgComponent(2)};
    }
  }

  return scaled;
}

void BM_ScaleComponents(benchmark::State& state) {
  auto randomPicture = MakeRandom();

  for (auto _ : state) {
    auto copy = randomPicture;
    benchmark::DoNotOptimize(ScaleDown(copy));
  }
}

BENCHMARK(BM_ScaleComponents)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
