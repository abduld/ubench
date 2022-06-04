#include "args.h"

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

  state.SetBytesProcessed(
      int64_t(state.iterations() * size * sizeof(ElemType)));
  state.SetItemsProcessed(int64_t(state.iterations() * size));
  state.counters["size"] = double(size);
}
BENCHMARK(VectorAdd_Naive)->SMALL_ARGS();
