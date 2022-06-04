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

template <int TileFactor>
static void VectorAdd_Naive_NO_SIMD_Aligned_C_Tiled(benchmark::State &state) {
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
    memset(a, 0, size * sizeof(ElemType));
    memset(b, 0, size * sizeof(ElemType));
    memset(c, 0, size * sizeof(ElemType));
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
    for (size_t i = 0; i < size; i += TileFactor) {
#pragma clang loop vectorize(disable) interleave(disable)
      for (size_t ii = 0; ii < TileFactor; ii++) {
        c[i + ii] = a[i + ii] + b[i + ii];
      }
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  state.counters["tile_factor"] = static_cast<double>(TileFactor);
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}

#define BENCHMARK_VectorAdd_Naive_NO_SIMD_Aligned_C_Tiled(TileFactor)          \
  BENCHMARK_TEMPLATE(VectorAdd_Naive_NO_SIMD_Aligned_C_Tiled, TileFactor)      \
      ->ARGS()                                                                 \
      ->Unit(benchmark::kMicrosecond)                                          \
      ->UseRealTime()
BENCHMARK_VectorAdd_Naive_NO_SIMD_Aligned_C_Tiled(4);
BENCHMARK_VectorAdd_Naive_NO_SIMD_Aligned_C_Tiled(8);
BENCHMARK_VectorAdd_Naive_NO_SIMD_Aligned_C_Tiled(16);
BENCHMARK_VectorAdd_Naive_NO_SIMD_Aligned_C_Tiled(32);
BENCHMARK_VectorAdd_Naive_NO_SIMD_Aligned_C_Tiled(64);

template <int TileFactor>
static void VectorAdd_Naive_Aligned_C_Tiled(benchmark::State &state) {
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
    memset(a, 0, size * sizeof(ElemType));
    memset(b, 0, size * sizeof(ElemType));
    memset(c, 0, size * sizeof(ElemType));
    state.ResumeTiming();
    for (size_t i = 0; i < size; i += TileFactor) {
      for (size_t ii = 0; ii < TileFactor; ii++) {
        c[i + ii] = a[i + ii] + b[i + ii];
      }
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  state.counters["tile_factor"] = static_cast<double>(TileFactor);
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}
#define BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled(TileFactor)                  \
  BENCHMARK_TEMPLATE(VectorAdd_Naive_Aligned_C_Tiled, TileFactor)              \
      ->ARGS()                                                                 \
      ->Unit(benchmark::kMicrosecond)                                          \
      ->UseRealTime()
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled(4);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled(8);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled(16);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled(32);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled(64);

template <int TileFactor, int UnrollFactor>
static void VectorAdd_Naive_Aligned_C_Tiled_Unroll(benchmark::State &state) {
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
    memset(a, 0, size * sizeof(ElemType));
    memset(b, 0, size * sizeof(ElemType));
    memset(c, 0, size * sizeof(ElemType));
    state.ResumeTiming();
    for (size_t i = 0; i < size; i += TileFactor) {
#pragma unroll(UnrollFactor)
      for (size_t ii = 0; ii < TileFactor; ii++) {
        c[i + ii] = a[i + ii] + b[i + ii];
      }
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  state.counters["tile_factor"] = static_cast<double>(TileFactor);
  state.counters["unroll_factor"] = static_cast<double>(TileFactor);
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}
#define BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(TileFactor,           \
                                                         UnrollFactor)         \
  BENCHMARK_TEMPLATE(VectorAdd_Naive_Aligned_C_Tiled_Unroll, TileFactor,       \
                     UnrollFactor)                                             \
      ->ARGS()                                                                 \
      ->Unit(benchmark::kMicrosecond)                                          \
      ->UseRealTime()
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(4, 2);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(4, 4);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(8, 4);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(8, 8);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(16, 8);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(32, 8);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(64, 8);
BENCHMARK_VectorAdd_Naive_Aligned_C_Tiled_Unroll(64, 32);