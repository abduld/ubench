//===- vector-add/naive.cpp ----------------------------------------------===//
//
// Performs a naive sequential vector addition.
//
//===---------------------------------------------------------------------===//
#include "args.h"
#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>

#include "stdlib.h"

/// Adds code sequentially using native C arrays.
static void VectorAdd_Naive_NO_SIMD_C(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  float *a = reinterpret_cast<float *>(calloc(sizeof(ElemType), size));
  float *b = reinterpret_cast<float *>(calloc(sizeof(ElemType), size));
  float *c = reinterpret_cast<float *>(malloc(size * sizeof(ElemType)));
  for (auto _ : state) {
    state.PauseTiming();
    memset(c, 0, size * sizeof(ElemType));
    benchmark::utils::WipeCache();
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  setInfoCounters(state);
  free(a);
  free(b);
  free(c);
}
BENCHMARK(VectorAdd_Naive_NO_SIMD_C)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using vectors (does not unbox the vector).
static void VectorAdd_Naive_NO_SIMD_Vector(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  std::vector<ElemType> a(size, 1);
  std::vector<ElemType> b(size, 1);
  std::vector<ElemType> c(size, 0);
  for (auto _ : state) {
    state.PauseTiming();
    memset(c.data(), 0, size * sizeof(ElemType));
    benchmark::utils::WipeCache();
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_NO_SIMD_Vector)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using vectors (unboxes the vector).
static void VectorAdd_Naive_NO_SIMD_VectorData(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  std::vector<ElemType> a(size, 1);
  std::vector<ElemType> b(size, 1);
  std::vector<ElemType> c(size, 0);
  auto aData = a.data(), bData = b.data(), cData = c.data();
  for (auto _ : state) {
    state.PauseTiming();
    memset(cData, 0, size * sizeof(ElemType));
    benchmark::utils::WipeCache();
    state.ResumeTiming();
#pragma clang loop vectorize(disable) interleave(disable)
    for (size_t i = 0; i < size; i++) {
      cData[i] = aData[i] + bData[i];
    }
    benchmark::DoNotOptimize(aData);
    benchmark::DoNotOptimize(bData);
    benchmark::DoNotOptimize(cData);
  }

  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_NO_SIMD_VectorData)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using native C arrays.
static void VectorAdd_Naive_C(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  float *a = reinterpret_cast<float *>(calloc(sizeof(ElemType), size));
  float *b = reinterpret_cast<float *>(calloc(sizeof(ElemType), size));
  float *c = reinterpret_cast<float *>(malloc(size * sizeof(ElemType)));
  for (auto _ : state) {
    state.PauseTiming();
    memset(c, 0, size * sizeof(ElemType));
    benchmark::utils::WipeCache();
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  setInfoCounters(state);
  free(a);
  free(b);
  free(c);
}
BENCHMARK(VectorAdd_Naive_C)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using vectors (does not unbox the vector).
static void VectorAdd_Naive_Vector(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  std::vector<ElemType> a(size, 1);
  std::vector<ElemType> b(size, 1);
  std::vector<ElemType> c(size, 0);
  for (auto _ : state) {
    state.PauseTiming();
    memset(c.data(), 0, size * sizeof(ElemType));
    benchmark::utils::WipeCache();
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      c[i] = a[i] + b[i];
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }

  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_Vector)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

/// Adds code sequentially using vectors (unboxes the vector).
static void VectorAdd_Naive_VectorData(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  std::vector<ElemType> a(size, 1);
  std::vector<ElemType> b(size, 1);
  std::vector<ElemType> c(size, 0);
  auto aData = a.data(), bData = b.data(), cData = c.data();
  for (auto _ : state) {
    state.PauseTiming();
    memset(cData, 0, size * sizeof(ElemType));
    benchmark::utils::WipeCache();
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      cData[i] = aData[i] + bData[i];
    }
    benchmark::DoNotOptimize(aData);
    benchmark::DoNotOptimize(bData);
    benchmark::DoNotOptimize(cData);
  }

  setInfoCounters(state);
}
BENCHMARK(VectorAdd_Naive_VectorData)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
