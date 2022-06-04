#include "args.h"
#include "utils/benchmark.h"

#include <benchmark/benchmark.h>
#include <vector>

using ElemType = float;

static void VectorAdd_Naive(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<ElemType> a(size, 1);
    std::vector<ElemType> b(size, 1);
    std::vector<ElemType> c(size, 0);
    auto aData = a.data(), bData = b.data(), cData = c.data();
    state.ResumeTiming();
    for (size_t i = 0; i < size; i++) {
      cData[i] = aData[i] + bData[i];
    }
    benchmark::DoNotOptimize(c);
  }

  if (uint64_t cpufreq = benchmark::utils::GetCurrentCpuFrequency())
    state.counters["cpufreq"] = static_cast<double>(cpufreq);

  state.counters["num_elements"] =
      benchmark::Counter(static_cast<double>(state.iterations() * size),
                         benchmark::Counter::kIsRate);

  const size_t bytes_per_iteration = 3 * size * sizeof(ElemType);
  state.counters["bytes"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}
BENCHMARK(VectorAdd_Naive)->SMALL_ARGS()->UseRealTime();
