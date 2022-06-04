//===- vector-add/naive.cpp ----------------------------------------------===//
//
// Performs a naive sequential vector addition using psimd.
//
//===---------------------------------------------------------------------===//
#include "args.h"
#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>

#include "stdlib.h"

/// Adds code sequentially using native C arrays.
static void VectorAdd_PSIMD(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  float *a = nullptr, *b = nullptr, *c = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    // free prior data from prior iterations of the loop
    free(a);
    free(b);
    free(c);
    a = reinterpret_cast<float *>(malloc(size * sizeof(ElemType)));
    b = reinterpret_cast<float *>(malloc(size * sizeof(ElemType)));
    c = reinterpret_cast<float *>(malloc(size * sizeof(ElemType)));
    std::fill(a, a + size, 1.0f);
    std::fill(b, b + size, 1.0f);
    std::fill(c, c + size, 0.0f);
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::DoNotOptimize(c);
  }

  setInfoCounters(state);
  free(a);
  free(b);
  free(c);
}
BENCHMARK(VectorAdd_Naive_C)->ARGS()->UseRealTime();
