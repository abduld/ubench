
#include <benchmark/benchmark.h>

#include "utils/benchmark.h"

using namespace benchmark::utils;

int main(int argc, char **argv) {
  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv))
    return 1;
  benchmark::AddCustomContext("MaxCacheSize",
                              std::to_string(GetMaxCacheSize()));
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();
  return 0;
}
