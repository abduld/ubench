#include "args.h"
#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>

#include "stdlib.h"

static void VectorAdd_Naive_C(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  float *a = nullptr, *b = nullptr, *c = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    free(a);
    free(b);
    free(c);
    a = reinterpret_cast<float *>(malloc(size * sizeof(ElemType)));
    b = reinterpret_cast<float *>(malloc(size * sizeof(ElemType)));
    c = reinterpret_cast<float *>(malloc(size * sizeof(ElemType)));
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      a[i] = b[i] + c[i];
    }
    benchmark::DoNotOptimize(c);
  }

  setInfoCounters(state);
  free(a);
  free(b);
  free(c);
}
BENCHMARK(VectorAdd_Naive_C)->SMALL_ARGS()->UseRealTime();

static void VectorAdd_Naive_Vector(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<ElemType> a(size, 1);
    std::vector<ElemType> b(size, 1);
    std::vector<ElemType> c(size, 0);
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::DoNotOptimize(c);
  }

  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_Vector)->SMALL_ARGS()->UseRealTime();

static void VectorAdd_Naive_VectorData(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<ElemType> a(size, 1);
    std::vector<ElemType> b(size, 1);
    std::vector<ElemType> c(size, 0);
    auto aData = a.data(), bData = b.data(), cData = c.data();
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      cData[i] = aData[i] + bData[i];
    }
    benchmark::DoNotOptimize(c);
  }

  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_VectorData)->SMALL_ARGS()->UseRealTime();
