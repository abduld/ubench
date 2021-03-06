include(FetchContent)

FetchContent_Declare(
  xnnpack
  GIT_REPOSITORY https://github.com/google/XNNPACK
  GIT_SHALLOW TRUE # get only last version
  GIT_PROGRESS TRUE # show progress of download
  USES_TERMINAL_DOWNLOAD TRUE # show progress in ninja generator
  PREFIX "${CMAKE_BINARY_DIR}"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/xnnpack"
)

set(XNNPACK_BUILD_TESTS OFF)
set(XNNPACK_BUILD_BENCHMARKS OFF)
set(XNNPACK_ENABLE_SPARSE OFF)
set(XNNPACK_LIBRARY_TYPE "static")
set(CPUINFO_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/ruy-src/third_party/cpuinfo)
set(CLOG_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/ruy-src/third_party/cpuinfo)
set(GOOGLEBENCHMARK_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/google-benchmark-src)

FetchContent_MakeAvailable(xnnpack)
