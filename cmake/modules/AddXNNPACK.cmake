include(FetchContent)

set(XNNPACK_BUILD_TESTS OFF)
set(XNNPACK_BUILD_BENCHMARKS OFF)
set(XNNPACK_ENABLE_SPARSE OFF)
set(GOOGLEBENCHMARK_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/google-benchmark-src)

FetchContent_Declare(
  XNNPACK
  GIT_REPOSITORY https://github.com/google/XNNPACK
  GIT_TAG main
  GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(XNNPACK)

add_subdirectory("${XNNPACK_SOURCE_DIR}" "${XNNPACK_BINARY_DIR}"
                 EXCLUDE_FROM_ALL)

include_directories(
  AFTER "${PTHREADPOOL_SOURCE_DIR}/include" "${FP16_SOURCE_DIR}/include"
  "${XNNPACK_SOURCE_DIR}/include" "${CPUINFO_SOURCE_DIR}/")
