#include "args.h"
#include "utils/benchmark.h"

#include <Eigen/Dense>
#include <Eigen/src/Core/util/Constants.h>
#include <benchmark/benchmark.h>
#include <vector>

#include "stdlib.h"

static void VectorAdd_Eigen(benchmark::State &state) {
  const auto size = 1ULL << static_cast<size_t>(state.range(0));
  float *a = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  float *b = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  float *c = reinterpret_cast<float *>(
      xsimd::aligned_malloc(size * sizeof(ElementType), Alignment));
  memset(a, 0, size * sizeof(ElementType));
  memset(b, 0, size * sizeof(ElementType));
  memset(c, 0, size * sizeof(ElementType));
  static_assert(Alignment == 8 || Alignment == 16 || Alignment == 32 ||
                    Alignment == 64 || Alignment == 128,
                "Alignment must be 8, 16, 32, 64, or 128");
  Eigen::Map<Eigen::RowVectorXf,
             Alignment == 8    ? Eigen::AlignmentType::Aligned8
             : Alignment == 16 ? Eigen::AlignmentType::Aligned16
             : Alignment == 32 ? Eigen::AlignmentType::Aligned32
             : Alignment == 64 ? Eigen::AlignmentType::Aligned64
                               : Eigen::AlignmentType::Aligned128>
      aEigen(a, Eigen::Index(size)), bEigen(b, Eigen::Index(size)),
      cEigen(c, Eigen::Index(size));
  for (auto _ : state) {
    state.PauseTiming();
    memset(c, 0, size * sizeof(ElementType));
    benchmark::utils::WipeCache(state);
    state.ResumeTiming();
    cEigen = aEigen + bEigen;
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(aEigen);
    benchmark::DoNotOptimize(bEigen);
    benchmark::DoNotOptimize(cEigen);
  }

  state.counters["alignment"] = static_cast<double>(Alignment);
  setInfoCounters(state);
  xsimd::aligned_free(a);
  xsimd::aligned_free(b);
  xsimd::aligned_free(c);
}
BENCHMARK(VectorAdd_Eigen)
    ->ARGS()
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();
