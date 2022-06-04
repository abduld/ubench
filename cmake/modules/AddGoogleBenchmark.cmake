include_guard()

include(FetchContent)

FetchContent_Declare(
  google-benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.6.1
  GIT_SHALLOW TRUE # get only last version
  GIT_PROGRESS TRUE # show progress of download
  USES_TERMINAL_DOWNLOAD TRUE # show progress in ninja generator
)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_ENABLE_LTO OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)

FetchContent_MakeAvailable(google-benchmark)
