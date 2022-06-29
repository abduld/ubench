
#include <cblas.h>

#include "args.h"

static void cblasGEMMImpl(const CBLAS_TRANSPOSE TransA,
                          const CBLAS_TRANSPOSE TransB, const int M,
                          const int N, const int K, const float alpha,
                          const float *A, const float *B, const float beta,
                          float *C) {
  static_assert(std::is_same_v(float, ElementType),
                "ElementType must be float");
  const int lda = (TransA == CblasNoTrans) ? K : M;
  const int ldb = (TransB == CblasNoTrans) ? N : K;

  cblas_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb,
              beta, C, N);
}

static void GEMM_CLBLAS(benchmark::State &state) {
  const ElementType one{1};
  const ElementType zero{0};

  const auto M = state.range(0);
  const auto N = state.range(1);
  const auto K = state.range(2);

  ElementType alpha{one};
  ElementType beta{zero};

  auto a = std::vector<ElementType>(M * K);
  auto b = std::vector<ElementType>(K * N);
  auto c = std::vector<ElementType>(M * N);
  std::fill(a.begin(), a.end(), one);
  std::fill(b.begin(), b.end(), one);
  std::fill(c.begin(), c.end(), zero);

  for (auto _ : state) {
    cblasGEMMImpl(CblasNoTrans, CblasNoTrans, M, N, K, alpha, a.data(),
                  b.data(), beta, c.data());
    benchmark::DoNotOptimize(c.data());
  }

  setInfoCounters(state);
}

BENCHMARK(GEMM_CLBLAS)->ARGS()->Unit(benchmark::kMicrosecond)->UseRealTime();
