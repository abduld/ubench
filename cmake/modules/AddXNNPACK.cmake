include(FetchContent)

set(XNNPACK_BUILD_TESTS OFF)
set(XNNPACK_BUILD_BENCHMARKS OFF)
set(XNNPACK_ENABLE_SPARSE OFF)
set(GOOGLEBENCHMARK_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/google-benchmark-src)

FetchContent_Declare(
  XNNPACK
  GIT_REPOSITORY https://github.com/google/XNNPACK
  GIT_SHALLOW TRUE # get only last version
  GIT_PROGRESS TRUE # show progress of download
  USES_TERMINAL_DOWNLOAD TRUE # show progress in ninja generator
)
FetchContent_MakeAvailable(XNNPACK)
