//===- vector-add/naive.cpp ----------------------------------------------===//
//
// Performs a naive sequential vector addition. The allocations are aligned,
// however.
//
//===---------------------------------------------------------------------===//
#include "args.h"
#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>

#include "stdlib.h"

/// Adds code sequentially using native C arrays.
static void VectorAdd_Naive_Aligned_C(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  float *a = nullptr, *b = nullptr, *c = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    // free prior data from prior iterations of the loop
    xsimd::aligned_free(a);
    xsimd::aligned_free(b);
    xsimd::aligned_free(c);
    a = reinterpret_cast<float *>(
        xsimd::aligned_malloc(size * sizeof(ElemType), Alignment));
    b = reinterpret_cast<float *>(
        xsimd::aligned_malloc(size * sizeof(ElemType), Alignment));
    c = reinterpret_cast<float *>(
        xsimd::aligned_malloc(size * sizeof(ElemType), Alignment));
    std::fill(a, a + size, 1.0f);
    std::fill(b, b + size, 1.0f);
    std::fill(c, c + size, 0.0f);
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}
BENCHMARK(VectorAdd_Naive_Aligned_C)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using vectors (does not unbox the vector).
static void VectorAdd_Naive_Aligned_Vector(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    aligned_vector<ElemType> a(size, 1);
    aligned_vector<ElemType> b(size, 1);
    aligned_vector<ElemType> c(size, 0);
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_Aligned_Vector)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using vectors (unboxes the vector).
static void VectorAdd_Naive_Aligned_VectorData(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    aligned_vector<ElemType> a(size, 1);
    aligned_vector<ElemType> b(size, 1);
    aligned_vector<ElemType> c(size, 0);
    auto aData = a.data(), bData = b.data(), cData = c.data();
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      cData[i] = aData[i] + bData[i];
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_Aligned_VectorData)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
