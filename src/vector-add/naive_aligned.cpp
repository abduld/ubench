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

/// Adds code sequentially using native aligned C arrays.
static void VectorAdd_Naive_NO_SIMD_Aligned_C(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  float *a = nullptr, *b = nullptr, *c = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    benchmark::utils::WipeCache();
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
    memset(a, 0, size * sizeof(ElemType));
    memset(b, 0, size * sizeof(ElemType));
    memset(c, 0, size * sizeof(ElemType));
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
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
BENCHMARK(VectorAdd_Naive_NO_SIMD_Aligned_C)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using aligned vectors (does not unbox the vector).
static void VectorAdd_Naive_NO_SIMD_Aligned_Vector(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    benchmark::utils::WipeCache();
    aligned_vector<ElemType> a(size, 1);
    aligned_vector<ElemType> b(size, 1);
    aligned_vector<ElemType> c(size, 0);
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
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
BENCHMARK(VectorAdd_Naive_NO_SIMD_Aligned_Vector)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using aligned vectors (unboxes the vector).
static void
VectorAdd_Naive_NO_SIMD_Aligned_VectorData(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    benchmark::utils::WipeCache();
    aligned_vector<ElemType> a(size, 1);
    aligned_vector<ElemType> b(size, 1);
    aligned_vector<ElemType> c(size, 0);
    auto aData = a.data(), bData = b.data(), cData = c.data();
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
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
BENCHMARK(VectorAdd_Naive_NO_SIMD_Aligned_VectorData)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
