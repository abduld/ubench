
#include <cblas.h>

#include "args.h"

static void cblasGEMMImpl(const CBLAS_TRANSPOSE TransA,
                          const CBLAS_TRANSPOSE TransB, const int M,
                          const int N, const int K, const float alpha,
                          const float *A, const float *B, const float beta,
                          float *C) {
  static_assert(std::is_same_v<float, ElementType>,
                "ElementType must be float");
  const int lda = (TransA == CblasNoTrans) ? K : M;
  const int ldb = (TransB == CblasNoTrans) ? N : K;

  cblas_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb,
              beta, C, N);
}

static void GEMM_CLBLAS(benchmark::State &state) {
  const ElementType one{1};
  const ElementType zero{0};

  const size_t M = static_cast<size_t>(state.range(0));
  const size_t N = static_cast<size_t>(state.range(1));
  const size_t K = static_cast<size_t>(state.range(2));

  ElementType alpha{one};
  ElementType beta{zero};

  auto a = std::vector<ElementType>(M * K);
  auto b = std::vector<ElementType>(K * N);
  auto c = std::vector<ElementType>(M * N);
  std::fill(a.begin(), a.end(), one);
  std::fill(b.begin(), b.end(), one);
  std::fill(c.begin(), c.end(), zero);

  for (auto _ : state) {
    cblasGEMMImpl(CblasNoTrans, CblasNoTrans, static_cast<int>(M),
                  static_cast<int>(N), static_cast<int>(K), alpha, a.data(),
                  b.data(), beta, c.data());
    benchmark::DoNotOptimize(c.data());
  }

  setInfoCounters(state);
}

BENCHMARK(GEMM_CLBLAS)
    ->SMALL_ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
