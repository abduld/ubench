
#include <cstddef>
#include <cstdint>

#include <benchmark/benchmark.h>
#include <xsimd/xsimd.hpp>
#include <xtensor/xtensor_config.hpp>

#define MODULE_NAME memcpy

#define PP_STRINGIFY_IMPL(x) #x
#define PP_STRINGIFY(x) PP_STRINGIFY_IMPL(x)

#define PP_CONCAT_IMPL(x, y) x##y
#define PP_CONCAT(x, y) PP_CONCAT_IMPL(x, y)

#define BENCHMARK_NAME(fn_name) PP_CONCAT(MODULE_NAME, PP_CONCAT(_, fn_name))

#define ADD_BENCHMARK_(fn) BENCHMARK(fn)->Unit(benchmark::kMicrosecond)
#define ADD_BENCHMARK(fn) ADD_BENCHMARK_(fn)

using ElementType = float;

static constexpr uint64_t N = 1 << 20;
static constexpr size_t XSIMD_DEFAULT_ALIGNMENT =
    xsimd::default_arch::alignment();

template <typename T>
using aligned_vector =
    std::vector<T, xsimd::aligned_allocator<T, XSIMD_DEFAULT_ALIGNMENT>>;

static void BENCHMARK_NAME(CPP_Naive_Memcpy)(benchmark::State &state) {
  aligned_vector<ElementType> Input(N, 1.0 / N), Output(N, 0);
  for (auto _ : state) {
    ElementType *inData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Input.data(), XSIMD_DEFAULT_ALIGNMENT));
    ElementType *outData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Output.data(), XSIMD_DEFAULT_ALIGNMENT));

    for (uint64_t ii = 0; ii < N; ii++) {
      outData[ii] = inData[ii];
    }
    benchmark::DoNotOptimize(Input.data());
    benchmark::DoNotOptimize(Output.data());
    benchmark::ClobberMemory();
  }

  state.counters["num_elements/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * N), benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = N * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}

ADD_BENCHMARK(BENCHMARK_NAME(CPP_Naive_Memcpy));

static void BENCHMARK_NAME(CPP_Naive_Memcpy_2)(benchmark::State &state) {
  aligned_vector<ElementType> Input(N, 1.0 / N), Output(N, 0);
  for (auto _ : state) {
    ElementType *inData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Input.data(), XSIMD_DEFAULT_ALIGNMENT));
    ElementType *outData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Output.data(), XSIMD_DEFAULT_ALIGNMENT));

    for (uint64_t ii = 0; ii < N; ii++) {
      *outData++ = *inData++;
    }
    benchmark::DoNotOptimize(Input.data());
    benchmark::DoNotOptimize(Output.data());
    benchmark::ClobberMemory();
  }

  state.counters["num_elements/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * N), benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = N * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}

ADD_BENCHMARK(BENCHMARK_NAME(CPP_Naive_Memcpy_2));

static void BENCHMARK_NAME(CPP_Memcpy)(benchmark::State &state) {
  aligned_vector<ElementType> Input(N, 1.0 / N), Output(N, 0);
  for (auto _ : state) {
    ElementType *inData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Input.data(), XSIMD_DEFAULT_ALIGNMENT));
    ElementType *outData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Output.data(), XSIMD_DEFAULT_ALIGNMENT));

    std::memcpy(outData, inData, sizeof(ElementType) * N);
    benchmark::DoNotOptimize(Input.data());
    benchmark::DoNotOptimize(Output.data());
    benchmark::ClobberMemory();
  }

  state.counters["num_elements/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * N), benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = N * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}

ADD_BENCHMARK(BENCHMARK_NAME(CPP_Memcpy));

static void BENCHMARK_NAME(CPP_Memcpy_XSIMD)(benchmark::State &state) {
  using simd_t = xsimd::simd_type<ElementType>;
  static constexpr std::size_t inc = simd_t::size;

  aligned_vector<ElementType> Input(N, 1.0 / N), Output(N, 0);
  for (auto _ : state) {
    ElementType *inData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Input.data(), XSIMD_DEFAULT_ALIGNMENT));
    ElementType *outData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Output.data(), XSIMD_DEFAULT_ALIGNMENT));
    for (uint64_t ii = 0; ii < N; ii += inc) {
      xsimd::store_aligned(&outData[ii], xsimd::load_aligned(&inData[ii]));
    }

    benchmark::DoNotOptimize(Input.data());
    benchmark::DoNotOptimize(Output.data());
    benchmark::ClobberMemory();
  }

  state.counters["num_elements/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * N), benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = N * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}
ADD_BENCHMARK(BENCHMARK_NAME(CPP_Memcpy_XSIMD));

static void BENCHMARK_NAME(CPP_Memcpy_XSIMD_2)(benchmark::State &state) {
  using simd_t = xsimd::simd_type<ElementType>;
  static constexpr std::size_t inc = simd_t::size;

  aligned_vector<ElementType> Input(N, 1.0 / N), Output(N, 0);
  for (auto _ : state) {
    ElementType *inData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Input.data(), XSIMD_DEFAULT_ALIGNMENT));
    ElementType *outData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Output.data(), XSIMD_DEFAULT_ALIGNMENT));
    for (uint64_t ii = 0; ii < N; ii += 4 * inc) {
#pragma unroll
      for (uint64_t jj = ii; jj < ii + 4 * inc; jj += inc) {
        xsimd::store_aligned(&outData[jj], xsimd::load_aligned(&inData[jj]));
      }
    }

    benchmark::DoNotOptimize(Input.data());
    benchmark::DoNotOptimize(Output.data());
    benchmark::ClobberMemory();
  }

  state.counters["num_elements/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * N), benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = N * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}

ADD_BENCHMARK(BENCHMARK_NAME(CPP_Memcpy_XSIMD_2));

static void BENCHMARK_NAME(CPP_Memcpy_Intrinsic)(benchmark::State &state) {
  aligned_vector<ElementType> Input(N, 1.0 / N), Output(N, 0);
  for (auto _ : state) {
    ElementType *inData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Input.data(), XSIMD_DEFAULT_ALIGNMENT));
    ElementType *outData = reinterpret_cast<ElementType *>(
        __builtin_assume_aligned(Output.data(), XSIMD_DEFAULT_ALIGNMENT));

    __builtin_memcpy(outData, inData, sizeof(ElementType) * N);
    benchmark::DoNotOptimize(Input.data());
    benchmark::DoNotOptimize(Output.data());
    benchmark::ClobberMemory();
  }

  state.counters["num_elements/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * N), benchmark::Counter::kIsRate);

  size_t bytes_per_iteration = N * sizeof(ElementType);
  state.counters["bytes/s"] = benchmark::Counter(
      static_cast<double>(state.iterations() * bytes_per_iteration),
      benchmark::Counter::kIsRate);
}

ADD_BENCHMARK(BENCHMARK_NAME(CPP_Memcpy_Intrinsic));
