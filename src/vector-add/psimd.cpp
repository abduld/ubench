#include "args.h"
#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <psimd.h>
#include <vector>

#include "stdlib.h"

static constexpr int SIMDWidth = sizeof(psimd_f32) / sizeof(float);

static void VectorAdd_Naive_PSIMD(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  assert(size % SIMDWidth == 0 && "size must be a multiple of SIMDWidth");
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
    for (size_t i = 0; i < size; i += SIMDWidth) {
      psimd_store_f32(
          c + i, psimd_add_f32(psimd_load_f32(a + i), psimd_load_f32(b + i)));
    }
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  state.counters["simd_width"] = static_cast<double>(SIMDWidth);
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}
BENCHMARK(VectorAdd_Naive_PSIMD)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using native aligned C arrays.
template <int TileFactor>
static void VectorAdd_Naive_PSIMD_Tiled(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  assert(size % (TileFactor * SIMDWidth) == 0 &&
         "size must be a multiple of TileFactor");
  assert(size % SIMDWidth == 0 && "size must be a multiple of SIMDWidth");
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
    for (size_t i = 0; i < size; i += TileFactor * SIMDWidth) {
      for (size_t ii = 0; ii < TileFactor * SIMDWidth; ii += SIMDWidth) {
        psimd_store_f32(c + i + ii, psimd_add_f32(psimd_load_f32(a + i + ii),
                                                  psimd_load_f32(b + i + ii)));
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
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}

#define BENCHMARK_PSIMD_TILED(TILE_FACTOR)                                     \
  BENCHMARK_TEMPLATE(VectorAdd_Naive_PSIMD_Tiled, TILE_FACTOR)                 \
      ->ARGS()                                                                 \
      ->Unit(benchmark::kMicrosecond)                                          \
      ->UseRealTime();

BENCHMARK_PSIMD_TILED(16);
BENCHMARK_PSIMD_TILED(16);
BENCHMARK_PSIMD_TILED(32);
