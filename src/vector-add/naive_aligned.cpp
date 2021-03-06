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
#pragma clang loop vectorize(disable) interleave(disable)
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::ClobberMemory();
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
  aligned_vector<ElementType> a(size, 1);
  aligned_vector<ElementType> b(size, 1);
  aligned_vector<ElementType> c(size, 0);
  for (auto _ : state) {
    state.PauseTiming();
    memset(c.data(), 0, size * sizeof(ElementType));
    benchmark::utils::WipeCache(state);
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::ClobberMemory();
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
  aligned_vector<ElementType> a(size, 1);
  aligned_vector<ElementType> b(size, 1);
  aligned_vector<ElementType> c(size, 0);
  auto aData = a.data(), bData = b.data(), cData = c.data();
  for (auto _ : state) {
    state.PauseTiming();
    memset(cData, 0, size * sizeof(ElementType));
    benchmark::utils::WipeCache(state);
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
    for (size_t i = 0; i < size; i++) {
      cData[i] = aData[i] + bData[i];
    }
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(aData);
    benchmark::DoNotOptimize(bData);
    benchmark::DoNotOptimize(cData);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_NO_SIMD_Aligned_VectorData)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using native aligned C arrays.
static void VectorAdd_Naive_Aligned_C(benchmark::State &state) {
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
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::ClobberMemory();
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

/// Adds code sequentially using aligned vectors (does not unbox the vector).
static void VectorAdd_Naive_Aligned_Vector(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  aligned_vector<ElementType> a(size, 1);
  aligned_vector<ElementType> b(size, 1);
  aligned_vector<ElementType> c(size, 0);
  for (auto _ : state) {
    state.PauseTiming();
    memset(c.data(), 0, size * sizeof(ElementType));
    benchmark::utils::WipeCache(state);
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::ClobberMemory();
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

/// Adds code sequentially using aligned vectors (unboxes the vector).
static void VectorAdd_Naive_Aligned_VectorData(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  aligned_vector<ElementType> a(size, 1);
  aligned_vector<ElementType> b(size, 1);
  aligned_vector<ElementType> c(size, 0);
  auto aData = a.data(), bData = b.data(), cData = c.data();
  for (auto _ : state) {
    state.PauseTiming();
    memset(cData, 0, size * sizeof(ElementType));
    benchmark::utils::WipeCache(state);
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      cData[i] = aData[i] + bData[i];
    }
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(aData);
    benchmark::DoNotOptimize(bData);
    benchmark::DoNotOptimize(cData);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_Aligned_VectorData)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
