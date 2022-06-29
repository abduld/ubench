include(FetchContent)

FetchContent_Declare(
  ruy
  GIT_REPOSITORY https://github.com/google/ruy
  GIT_SHALLOW FALSE # we need to download the ruy submodules
  GIT_PROGRESS TRUE # show progress of download
  USES_TERMINAL_DOWNLOAD TRUE # show progress in ninja generator
)

set(RUY_MINIMAL_BUILD ON)
set(RUY_PROFILER OFF)
set(RUY_ENABLE_INSTALL OFF)

FetchContent_MakeAvailable(ruy)
