#include "args.h"

#include <Eigen/Dense>
#include <Eigen/src/Core/util/Constants.h>

static void GEMM_Eigen(benchmark::State &state) {
  const Eigen::Index M = state.range(0);
  const Eigen::Index N = state.range(1);
  const Eigen::Index K = state.range(2);

  Eigen::MatrixXf a = Eigen::MatrixXf::Ones(M, K);
  Eigen::MatrixXf b = Eigen::MatrixXf::Ones(K, N);
  Eigen::MatrixXf c = Eigen::MatrixXf::Zero(M, N);

  for (auto _ : state) {
    c.noalias() += a * b;
    benchmark::DoNotOptimize(c.data());
  }

  setInfoCounters(state);
}

BENCHMARK(GEMM_Eigen)->ARGS()->Unit(benchmark::kMicrosecond)->UseRealTime();
