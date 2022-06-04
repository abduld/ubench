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
template <int SIMDWidth>
static void VectorAdd_Naive_OMP_SIMD(benchmark::State &state) {
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
    std::fill(a, a + size, 1.0f);
    std::fill(b, b + size, 1.0f);
    std::fill(c, c + size, 0.0f);
    state.ResumeTiming();
#pragma omp simd simdlen(SIMDWidth)
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
BENCHMARK_TEMPLATE(VectorAdd_Naive_OMP_SIMD, 2)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
BENCHMARK_TEMPLATE(VectorAdd_Naive_OMP_SIMD, 4)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
BENCHMARK_TEMPLATE(VectorAdd_Naive_OMP_SIMD, 8)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using native aligned C arrays.
template <int TileFactor, int SIMDWidth>
static void VectorAdd_Naive_OMP_SIMD_Tiled(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  assert(size % TileFactor == 0 && "size must be a multiple of TileFactor");
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
    std::fill(a, a + size, 1.0f);
    std::fill(b, b + size, 1.0f);
    std::fill(c, c + size, 0.0f);
    state.ResumeTiming();
    for (size_t i = 0; i < size; i += TileFactor) {
#pragma omp simd simdlen(SIMDWidth)
      for (size_t ii = 0; ii < TileFactor; ii++) {
        c[i + ii] = a[i + ii] + b[i + ii];
      }
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

#define BENCHMARK_OMP_SIMD_TILED(TILE_FACTOR, SIMD_WIDTH)                      \
  BENCHMARK_TEMPLATE(VectorAdd_Naive_OMP_SIMD_Tiled, TILE_FACTOR, SIMD_WIDTH)  \
      ->ARGS()                                                                 \
      ->Unit(benchmark::kMicrosecond)                                          \
      ->UseRealTime();

BENCHMARK_OMP_SIMD_TILED(2, 2);
BENCHMARK_OMP_SIMD_TILED(16, 8);
BENCHMARK_OMP_SIMD_TILED(16, 8);
BENCHMARK_OMP_SIMD_TILED(32, 8);
