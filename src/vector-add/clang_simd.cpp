#include "args.h"
#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>

#include "stdlib.h"

template <int SIMDWidth, int InterleaveCount>
static void VectorAdd_CLANG_SIMD(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  float *a = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  float *b = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  float *c = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  memset(a, 0, size * sizeof(ElementType));
  memset(b, 0, size * sizeof(ElementType));
  memset(c, 0, size * sizeof(ElementType));
  for (auto _ : state) {
    state.PauseTiming();
    memset(c, 0, size * sizeof(ElementType));
    benchmark::utils::WipeCache(state);
    state.ResumeTiming();
#pragma clang loop vectorize_width(SIMDWidth) interleave_count(InterleaveCount)
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  state.counters["simd_width"] = static_cast<double>(SIMDWidth);
  state.counters["interleave_count"] = static_cast<double>(InterleaveCount);
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}

#define VectorAdd_CLANG_SIMD(SIMD_WIDTH, INTERLEAVE_COUNT)                     \
  BENCHMARK_TEMPLATE(VectorAdd_CLANG_SIMD, SIMD_WIDTH, INTERLEAVE_COUNT)       \
      ->ARGS()                                                                 \
      ->Unit(benchmark::kMicrosecond)                                          \
      ->UseRealTime();

VectorAdd_CLANG_SIMD(2, 1);
VectorAdd_CLANG_SIMD(4, 1);
VectorAdd_CLANG_SIMD(4, 2);
VectorAdd_CLANG_SIMD(8, 1);
VectorAdd_CLANG_SIMD(8, 2);
VectorAdd_CLANG_SIMD(8, 2);

template <int TileFactor, int SIMDWidth, int InterleaveCount>
static void VectorAdd_CLANG_SIMD_Tiled(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  assert(size % TileFactor == 0 && "size must be a multiple of TileFactor");
  float *a = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  float *b = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  float *c = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  memset(a, 0, size * sizeof(ElementType));
  memset(b, 0, size * sizeof(ElementType));
  memset(c, 0, size * sizeof(ElementType));
  for (auto _ : state) {
    state.PauseTiming();
    memset(c, 0, size * sizeof(ElementType));
    benchmark::utils::WipeCache(state);
    state.ResumeTiming();
    for (size_t i = 0; i < size; i += TileFactor) {
#pragma clang loop vectorize_width(SIMDWidth) interleave_count(InterleaveCount)
      for (size_t ii = 0; ii < TileFactor; ii++) {
        c[i + ii] = a[i + ii] + b[i + ii];
      }
    }
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  state.counters["tile_factor"] = static_cast<double>(TileFactor);
  state.counters["simd_width"] = static_cast<double>(SIMDWidth);
  state.counters["interleave_count"] = static_cast<double>(InterleaveCount);
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}

#define BENCHMARK_CLANG_SIMD_TILED(TILE_FACTOR, SIMD_WIDTH, INTERLEAVE_COUNT)  \
  BENCHMARK_TEMPLATE(VectorAdd_CLANG_SIMD_Tiled, TILE_FACTOR, SIMD_WIDTH,      \
                     INTERLEAVE_COUNT)                                         \
      ->ARGS()                                                                 \
      ->Unit(benchmark::kMicrosecond)                                          \
      ->UseRealTime();

BENCHMARK_CLANG_SIMD_TILED(2, 2, 1);
BENCHMARK_CLANG_SIMD_TILED(16, 4, 1);
BENCHMARK_CLANG_SIMD_TILED(16, 4, 1);
BENCHMARK_CLANG_SIMD_TILED(32, 4, 1);
BENCHMARK_CLANG_SIMD_TILED(16, 8, 1);
BENCHMARK_CLANG_SIMD_TILED(16, 8, 1);
BENCHMARK_CLANG_SIMD_TILED(32, 8, 1);

BENCHMARK_CLANG_SIMD_TILED(2, 2, 2);
BENCHMARK_CLANG_SIMD_TILED(16, 4, 2);
BENCHMARK_CLANG_SIMD_TILED(16, 4, 2);
BENCHMARK_CLANG_SIMD_TILED(32, 4, 2);
BENCHMARK_CLANG_SIMD_TILED(16, 8, 2);
BENCHMARK_CLANG_SIMD_TILED(16, 8, 2);
BENCHMARK_CLANG_SIMD_TILED(32, 8, 2);

BENCHMARK_CLANG_SIMD_TILED(2, 2, 4);
BENCHMARK_CLANG_SIMD_TILED(16, 4, 4);
BENCHMARK_CLANG_SIMD_TILED(16, 4, 4);
BENCHMARK_CLANG_SIMD_TILED(32, 4, 4);
BENCHMARK_CLANG_SIMD_TILED(16, 8, 4);
BENCHMARK_CLANG_SIMD_TILED(16, 8, 4);
BENCHMARK_CLANG_SIMD_TILED(32, 8, 4);
