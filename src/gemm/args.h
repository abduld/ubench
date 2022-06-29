#pragma once

#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>

using ElementType = float;

static void setInfoCounters(benchmark::State &state) {
  const auto M = state.range(0);
  const auto N = state.range(1);
  const auto K = state.range(2);

  auto aSize = M * K, bSize = K * N, cSize = M * N;
  auto size = aSize * bSize * cSize;

  if (uint64_t cpufreq = benchmark::utils::GetCurrentCpuFrequency())
    state.counters["cpufreq"] = static_cast<double>(cpufreq);

  state.counters["M"] = double(M);
  state.counters["N"] = double(N);
  state.counters["K"] = double(K);
  state.counters["flops"] = benchmark::Counter(
      double(2 * M * N * K), benchmark::Counter::kAvgThreadsRate);
  state.counters["num_elements"] = double(size);
  state.counters["num_elements/s"] =
      benchmark::Counter(static_cast<double>(state.iterations() * size),
                         benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = size * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}

// M, N, K

#define SMALL_ARGS()                                                           \
  Args({1000, 1, 1})                                                           \
      ->Args({128, 169, 1728})                                                 \
      ->Args({128, 729, 1200})                                                 \
      ->Args({192, 169, 1728})                                                 \
      ->Args({256, 169, 1})                                                    \
      ->Args({256, 729, 1})                                                    \
      ->Args({384, 169, 1})                                                    \
      ->Args({384, 169, 2304})                                                 \
      ->Args({50, 1000, 1})                                                    \
      ->Args({50, 1000, 4096})                                                 \
      ->Args({50, 4096, 1})                                                    \
      ->Args({50, 4096, 4096})                                                 \
      ->ArgNames({"M", "N", "K"})

#define ALL_ARGS()                                                             \
  Args({1000, 1, 1})                                                           \
      ->Args({128, 169, 1728})                                                 \
      ->Args({128, 729, 1200})                                                 \
      ->Args({192, 169, 1728})                                                 \
      ->Args({256, 169, 1})                                                    \
      ->Args({256, 729, 1})                                                    \
      ->Args({384, 169, 1})                                                    \
      ->Args({384, 169, 2304})                                                 \
      ->Args({50, 1000, 1})                                                    \
      ->Args({50, 1000, 4096})                                                 \
      ->Args({50, 4096, 1})                                                    \
      ->Args({50, 4096, 4096})                                                 \
      ->Args({50, 4096, 9216})                                                 \
      ->Args({96, 3025, 1})                                                    \
      ->Args({96, 3025, 363})                                                  \
      ->Args({5124, 700, 2048})                                                \
      ->Args({35, 700, 2048})                                                  \
      ->Args({5124, 700, 2560})                                                \
      ->Args({35, 700, 2560})                                                  \
      ->Args({5124, 1500, 2048})                                               \
      ->Args({35, 1500, 2048})                                                 \
      ->Args({5124, 1500, 2560})                                               \
      ->Args({35, 1500, 2560})                                                 \
      ->Args({7680, 1, 2560})                                                  \
      ->Args({7680, 2, 2560})                                                  \
      ->Args({7680, 4, 2560})                                                  \
      ->Args({3072, 1, 1024})                                                  \
      ->Args({3072, 2, 1024})                                                  \
      ->Args({3072, 4, 1024})                                                  \
      ->Args({512, 1, 500000})                                                 \
      ->Args({1024, 1, 500000})                                                \
      ->Args({512, 2, 500000})                                                 \
      ->Args({1024, 2, 500000})                                                \
      ->Args({512, 4, 500000})                                                 \
      ->Args({1024, 4, 500000})                                                \
      ->Args({1024, 700, 512})                                                 \
      ->Args({7680, 1500, 2560})                                               \
      ->Args({6144, 1500, 2048})                                               \
      ->Args({4608, 1500, 1536})                                               \
      ->Args({8448, 1500, 2816})                                               \
      ->Args({3072, 1500, 1024})                                               \
      ->Args({7680, 3000, 2560})                                               \
      ->Args({6144, 3000, 2048})                                               \
      ->Args({4608, 3000, 1536})                                               \
      ->Args({8448, 3000, 2816})                                               \
      ->Args({3072, 3000, 1024})                                               \
      ->Args({7680, 6000, 2560})                                               \
      ->Args({6144, 6000, 2048})                                               \
      ->Args({4608, 6000, 1536})                                               \
      ->Args({8448, 6000, 2816})                                               \
      ->Args({3072, 6000, 1024})                                               \
      ->Args({6144, 1, 2048})                                                  \
      ->Args({4608, 1, 1536})                                                  \
      ->Args({8448, 1, 2816})                                                  \
      ->Args({6144, 2, 2048})                                                  \
      ->Args({4608, 2, 1536})                                                  \
      ->Args({8448, 2, 2816})                                                  \
      ->Args({6144, 4, 2048})                                                  \
      ->Args({4608, 4, 1536})                                                  \
      ->Args({8448, 4, 2816})                                                  \
      ->Args({512, 1500, 2816})                                                \
      ->Args({512, 1500, 2048})                                                \
      ->Args({512, 1500, 2560})                                                \
      ->Args({512, 1500, 1536})                                                \
      ->Args({1024, 1500, 2816})                                               \
      ->Args({1024, 1500, 2048})                                               \
      ->Args({1024, 1500, 2560})                                               \
      ->Args({1024, 1500, 1536})                                               \
      ->Args({512, 1, 512})                                                    \
      ->Args({1024, 1, 512})                                                   \
      ->Args({512, 3000, 2816})                                                \
      ->Args({512, 3000, 2048})                                                \
      ->Args({512, 3000, 2560})                                                \
      ->Args({512, 3000, 1536})                                                \
      ->Args({1024, 3000, 2816})                                               \
      ->Args({1024, 3000, 2048})                                               \
      ->Args({1024, 3000, 2560})                                               \
      ->Args({1024, 3000, 1536})                                               \
      ->Args({512, 2, 512})                                                    \
      ->Args({1024, 2, 512})                                                   \
      ->Args({512, 6000, 2816})                                                \
      ->Args({512, 6000, 2048})                                                \
      ->Args({512, 6000, 2560})                                                \
      ->Args({512, 6000, 1536})                                                \
      ->Args({1024, 6000, 2816})                                               \
      ->Args({1024, 6000, 2048})                                               \
      ->Args({1024, 6000, 2560})                                               \
      ->Args({1024, 6000, 1536})                                               \
      ->Args({512, 4, 512})                                                    \
      ->Args({1024, 4, 512})                                                   \
      ->ArgNames({"M", "N", "K"})
