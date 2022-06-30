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
  state.counters["num_elements/s"] = benchmark::Counter(
      static_cast<double>(state.iterations()) * static_cast<double>(size),
      benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = size_t(size) * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}

// M, N, K

#define ARGS ALL_ARGS

#define VERY_SMALL_ARGS()                                                      \
  Args({1000, 1, 1})->Args({128, 169, 1728})->ArgNames({"M", "N", "K"})

#define SMALL_ARGS()                                                           \
  Args({16, 16, 16})                                                           \
      ->Args({32, 32, 32})                                                     \
      ->Args({48, 48, 48})                                                     \
      ->Args({64, 64, 64})                                                     \
      ->Args({96, 96, 96})                                                     \
      ->Args({128, 128, 128})                                                  \
      ->Args({192, 192, 192})                                                  \
      ->Args({256, 256, 256})                                                  \
      ->Args({512, 512, 512})                                                  \
      ->Args({768, 768, 768})                                                  \
      ->Args({1024, 1024, 1024})                                               \
      ->Args({1536, 1536, 1536})                                               \
      ->Args({2048, 2048, 2048})                                               \
      ->Args({2560, 2560, 2560})                                               \
      ->Args({3072, 3072, 3072})                                               \
      ->Args({3584, 3584, 3584})                                               \
      ->Args({4096, 4096, 4096})                                               \
      ->Args({5120, 5120, 5120})                                               \
      ->Args({6144, 6144, 6144})                                               \
      ->Args({7168, 7168, 7168})                                               \
      ->Args({8192, 8192, 8192})                                               \
      ->Args({9216, 9216, 9216})                                               \
      ->Args({9728, 9728, 9728})                                               \
      ->Args({10240, 10240, 10240})                                            \
      ->ArgNames({"M", "N", "K"})

#define ALL_ARGS()                                                             \
  Args({16, 16, 16})                                                           \
      ->Args({32, 32, 32})                                                     \
      ->Args({48, 48, 48})                                                     \
      ->Args({64, 64, 64})                                                     \
      ->Args({96, 96, 96})                                                     \
      ->Args({128, 128, 128})                                                  \
      ->Args({192, 192, 192})                                                  \
      ->Args({256, 256, 256})                                                  \
      ->Args({512, 512, 512})                                                  \
      ->Args({768, 768, 768})                                                  \
      ->Args({1024, 1024, 1024})                                               \
      ->Args({1536, 1536, 1536})                                               \
      ->Args({2048, 2048, 2048})                                               \
      ->Args({2560, 2560, 2560})                                               \
      ->Args({3072, 3072, 3072})                                               \
      ->Args({3584, 3584, 3584})                                               \
      ->Args({4096, 4096, 4096})                                               \
      ->Args({5120, 5120, 5120})                                               \
      ->Args({6144, 6144, 6144})                                               \
      ->Args({7168, 7168, 7168})                                               \
      ->Args({8192, 8192, 8192})                                               \
      ->Args({9216, 9216, 9216})                                               \
      ->Args({9728, 9728, 9728})                                               \
      ->Args({10240, 10240, 10240})                                            \
      ->Args({10752, 10752, 10752})                                            \
      ->Args({11264, 11264, 11264})                                            \
      ->Args({11776, 11776, 11776})                                            \
      ->Args({12288, 12288, 12288})                                            \
      ->Args({12800, 12800, 12800})                                            \
      ->Args({13312, 13312, 13312})                                            \
      ->Args({13824, 13824, 13824})                                            \
      ->Args({14336, 14336, 14336})                                            \
      ->Args({14848, 14848, 14848})                                            \
      ->Args({15360, 15360, 15360})                                            \
      ->Args({15872, 15872, 15872})                                            \
      ->Args({16384, 16384, 16384})                                            \
      ->Args({16896, 16896, 16896})                                            \
      ->Args({17408, 17408, 17408})                                            \
      ->Args({17920, 17920, 17920})                                            \
      ->Args({18432, 18432, 18432})                                            \
      ->Args({18944, 18944, 18944})                                            \
      ->Args({19456, 19456, 19456})                                            \
      ->Args({19968, 19968, 19968})                                            \
      ->Args({20480, 20480, 20480})                                            \
      ->Args({20992, 20992, 20992})                                            \
      ->Args({21504, 21504, 21504})                                            \
      ->Args({22016, 22016, 22016})                                            \
      ->Args({22528, 22528, 22528})                                            \
      ->Args({23040, 23040, 23040})                                            \
      ->Args({23552, 23552, 23552})                                            \
      ->Args({24064, 24064, 24064})                                            \
      ->Args({24576, 24576, 24576})                                            \
      ->Args({25088, 25088, 25088})                                            \
      ->Args({25600, 25600, 25600})                                            \
      ->Args({26112, 26112, 26112})                                            \
      ->Args({26624, 26624, 26624})                                            \
      ->Args({27136, 27136, 27136})                                            \
      ->Args({27648, 27648, 27648})                                            \
      ->Args({28160, 28160, 28160})                                            \
      ->ArgNames({"M", "N", "K"})
