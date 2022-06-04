#pragma once

#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>
#include <xsimd/xsimd.hpp>
#include <xtensor/xtensor_config.hpp>

using ElementType = float;

static constexpr size_t Alignment = xsimd::default_arch::alignment();

template <typename T>
using aligned_vector = std::vector<T, xsimd::aligned_allocator<T, Alignment>>;

#define ARGS() DenseRange(23, 23, 1)->ArgName("log2(N)")

static void setInfoCounters(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  if (uint64_t cpufreq = benchmark::utils::GetCurrentCpuFrequency())
    state.counters["cpufreq"] = static_cast<double>(cpufreq);

  state.counters["log2(num_elements)"] = double(state.range(0));
  state.counters["num_elements"] = double(size);
  state.counters["num_elements/s"] =
      benchmark::Counter(static_cast<double>(state.iterations() * size),
                         benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = size_t(3) * size * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}

// static void CustomArguments(benchmark::internal::Benchmark *b) {
//   b->Args({0})->ArgName("log2(N)");
//   for (int i = 12; i <= 33; ++i)
//     b->Args({i})->ArgName("log2(N)");
// }

// #define BYTE_ARGS() Apply(CustomArguments)
