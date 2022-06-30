#include "args.h"

#include "mlas.h"

static void GEMM_MLAS(benchmark::State &state) {
  const ElementType one{1};
  const ElementType zero{0};

  const auto M = static_cast<size_t>(state.range(0));
  const auto N = static_cast<size_t>(state.range(1));
  const auto K = static_cast<size_t>(state.range(2));

  ElementType alpha{one};
  ElementType beta{zero};

  auto a = std::vector<ElementType>(M * K);
  auto b = std::vector<ElementType>(K * N);
  auto c = std::vector<ElementType>(M * N);
  std::fill(a.begin(), a.end(), one);
  std::fill(b.begin(), b.end(), one);
  std::fill(c.begin(), c.end(), zero);

  for (auto _ : state) {
    MlasGemm(CblasNoTrans, CblasNoTrans, M, N, K, alpha, a.data(), K, b.data(),
             N, beta, c.data(), N, nullptr);
  }

  setInfoCounters(state);
}

BENCHMARK(GEMM_MLAS)
    ->ARGS()
    ->ArgNames({"M", "N", "K"})
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
