#include "args.h"

#include "ruy/ruy.h"

static void GEMM_Ruy(benchmark::State &state) {
  const ElementType one{1};
  const ElementType zero{0};

  const auto M = static_cast<size_t>(state.range(0));
  const auto N = static_cast<size_t>(state.range(1));
  const auto K = static_cast<size_t>(state.range(2));

  auto aVec = std::vector<ElementType>(M * K);
  auto bVec = std::vector<ElementType>(K * N);
  auto cVec = std::vector<ElementType>(M * N);
  std::fill(aVec.begin(), aVec.end(), one);
  std::fill(bVec.begin(), bVec.end(), one);
  std::fill(cVec.begin(), cVec.end(), zero);

  ruy::Matrix<ElementType> a;
  ruy::MakeSimpleLayout(static_cast<int>(M), static_cast<int>(K),
                        ruy::Order::kRowMajor, a.mutable_layout());
  a.set_data(aVec.data());

  ruy::Matrix<ElementType> b;
  ruy::MakeSimpleLayout(static_cast<int>(K), static_cast<int>(N),
                        ruy::Order::kRowMajor, b.mutable_layout());
  b.set_data(bVec.data());

  ruy::Matrix<ElementType> c;
  ruy::MakeSimpleLayout(static_cast<int>(M), static_cast<int>(N),
                        ruy::Order::kRowMajor, c.mutable_layout());
  c.set_data(cVec.data());

  ruy::Context context;
  for (auto _ : state) {
    ruy::MulParams<float, float> mul_params;
    ruy::Mul(a, b, mul_params, &context, &c);
    benchmark::DoNotOptimize(c.data());
  }

  setInfoCounters(state);
}

BENCHMARK(GEMM_Ruy)
    ->ARGS()
    ->ArgNames({"M", "N", "K"})
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
