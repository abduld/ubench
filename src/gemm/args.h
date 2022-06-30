#pragma once

#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>

using ElementType = float;

static void setInfoCounters(benchmark::State &state) {
  const auto M = state.range(0);
  const auto N = state.range(1);
  const auto K = state.range(2);

  if (uint64_t cpufreq = benchmark::utils::GetCurrentCpuFrequency())
    state.counters["cpufreq"] = static_cast<double>(cpufreq);

  state.counters["M"] = double(M);
  state.counters["N"] = double(N);
  state.counters["K"] = double(K);
  state.counters["flops"] = benchmark::Counter(
      double(2 * M * N * K), benchmark::Counter::kAvgThreadsRate);
}

// M, N, K

#define ARGS()                                                                 \
  ARG_INCEPTION_V3()                                                           \
      ->ARG_MOBILENET_V3()                                                     \
      ->ARG_RESNET50()                                                         \
      ->ARG_SMALL_SIZES()                                                      \
      ->ARG_MOBILEBERT()

#define ARG_INCEPTION_V3()                                                     \
  Args({22500, 32, 27})                                                        \
      ->Args({22201, 32, 288})                                                 \
      ->Args({22201, 64, 288})                                                 \
      ->Args({5625, 80, 64})                                                   \
      ->Args({5329, 192, 720})                                                 \
      ->Args({1369, 64, 192})                                                  \
      ->Args({1369, 48, 192})                                                  \
      ->Args({1369, 64, 1200})                                                 \
      ->Args({1369, 96, 576})                                                  \
      ->Args({1369, 96, 864})                                                  \
      ->Args({1369, 32, 192})                                                  \
      ->Args({1369, 64, 256})                                                  \
      ->Args({1369, 48, 256})                                                  \
      ->Args({1369, 64, 288})                                                  \
      ->Args({1369, 48, 288})                                                  \
      ->Args({324, 384, 2592})                                                 \
      ->Args({324, 96, 864})                                                   \
      ->Args({361, 192, 768})                                                  \
      ->Args({361, 128, 768})                                                  \
      ->Args({361, 128, 896})                                                  \
      ->Args({361, 192, 896})                                                  \
      ->Args({361, 160, 768})                                                  \
      ->Args({361, 160, 1120})                                                 \
      ->Args({361, 192, 1120})                                                 \
      ->Args({361, 192, 1344})                                                 \
      ->Args({81, 320, 1728})                                                  \
      ->Args({81, 192, 1728})                                                  \
      ->Args({100, 320, 1280})                                                 \
      ->Args({100, 384, 1280})                                                 \
      ->Args({100, 384, 1152})                                                 \
      ->Args({100, 448, 1280})                                                 \
      ->Args({100, 384, 4032})                                                 \
      ->Args({100, 192, 1280})                                                 \
      ->Args({100, 320, 2048})                                                 \
      ->Args({100, 384, 2048})                                                 \
      ->Args({100, 448, 2048})                                                 \
      ->Args({100, 192, 2048})                                                 \
      ->Args({9, 1001, 2048})

#define ARG_MOBILENET_V3()                                                     \
  Args({12544, 16, 27})                                                        \
      ->Args({12544, 16, 16})                                                  \
      ->Args({12544, 64, 16})                                                  \
      ->Args({3136, 24, 64})                                                   \
      ->Args({3136, 72, 24})                                                   \
      ->Args({3136, 24, 72})                                                   \
      ->Args({1, 24, 72})                                                      \
      ->Args({1, 72, 24})                                                      \
      ->Args({784, 40, 72})                                                    \
      ->Args({784, 120, 40})                                                   \
      ->Args({1, 32, 120})                                                     \
      ->Args({1, 120, 32})                                                     \
      ->Args({784, 40, 120})                                                   \
      ->Args({784, 240, 40})                                                   \
      ->Args({196, 80, 240})                                                   \
      ->Args({196, 200, 80})                                                   \
      ->Args({196, 80, 200})                                                   \
      ->Args({196, 184, 80})                                                   \
      ->Args({196, 80, 184})                                                   \
      ->Args({196, 184, 80})                                                   \
      ->Args({196, 80, 184})                                                   \
      ->Args({196, 480, 80})                                                   \
      ->Args({1, 120, 480})                                                    \
      ->Args({1, 480, 120})                                                    \
      ->Args({196, 112, 480})                                                  \
      ->Args({196, 672, 112})                                                  \
      ->Args({1, 168, 672})                                                    \
      ->Args({1, 672, 168})                                                    \
      ->Args({196, 112, 672})                                                  \
      ->Args({49, 160, 672})                                                   \
      ->Args({49, 960, 160})                                                   \
      ->Args({1, 240, 960})                                                    \
      ->Args({1, 960, 240})                                                    \
      ->Args({49, 160, 960})                                                   \
      ->Args({1, 1280, 960})                                                   \
      ->Args({1, 1001, 1280})

#define ARG_RESNET50()                                                         \
  Args({12544, 64, 147})                                                       \
      ->Args({3136, 64, 64})                                                   \
      ->Args({3136, 64, 576})                                                  \
      ->Args({3136, 256, 64})                                                  \
      ->Args({3136, 64, 256})                                                  \
      ->Args({3136, 128, 256})                                                 \
      ->Args({784, 128, 1152})                                                 \
      ->Args({784, 512, 128})                                                  \
      ->Args({784, 512, 256})                                                  \
      ->Args({784, 128, 512})                                                  \
      ->Args({784, 256, 512})                                                  \
      ->Args({196, 256, 2304})                                                 \
      ->Args({196, 1024, 256})                                                 \
      ->Args({196, 1024, 512})                                                 \
      ->Args({196, 256, 1024})                                                 \
      ->Args({196, 512, 1024})                                                 \
      ->Args({49, 512, 4608})                                                  \
      ->Args({49, 2048, 512})                                                  \
      ->Args({49, 2048, 1024})                                                 \
      ->Args({49, 512, 2048})

#define ARG_SMALL_SIZES()                                                      \
  Args({18, 32, 96})                                                           \
      ->Args({24, 64, 96})                                                     \
      ->Args({24, 64, 256})                                                    \
      ->Args({48, 64, 128})                                                    \
      ->Args({192, 64, 128})                                                   \
      ->Args({192, 128, 128})                                                  \
      ->Args({480, 512, 16})                                                   \
      ->Args({384, 256, 256})

#define ARG_MOBILEBERT()                                                       \
  Args({384, 384, 512})                                                        \
      ->Args({384, 128, 128})                                                  \
      ->Args({384, 128, 512})                                                  \
      ->Args({384, 512, 128})                                                  \
      ->Args({384, 512, 2})                                                    \
      ->Args({384, 384, 32})
