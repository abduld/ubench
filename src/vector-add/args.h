#pragma once

#include <benchmark/benchmark.h>

#define SMALL_ARGS() DenseRange(8, 33, 1)->ArgName("log2(N)")

// static void CustomArguments(benchmark::internal::Benchmark *b) {
//   b->Args({0})->ArgName("log2(N)");
//   for (int i = 12; i <= 33; ++i)
//     b->Args({i})->ArgName("log2(N)");
// }

// #define BYTE_ARGS() Apply(CustomArguments)
