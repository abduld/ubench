include(FetchContent)

FetchContent_Declare(
  XTensor
  GIT_REPOSITORY https://github.com/xtensor-stack/xtensor.git
  GIT_TAG 0.24.2
  GIT_SHALLOW TRUE # get only last version
  GIT_PROGRESS TRUE # show progress of download
  USES_TERMINAL_DOWNLOAD TRUE # show progress in ninja generator
)

set(XTENSOR_USE_XSIMD ON)
set(XTENSOR_USE_OPENMP ON)
set(XTENSOR_DEFAULT_ALLOCATOR ON)

FetchContent_MakeAvailable(XTensor)
