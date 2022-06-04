include_guard()

include(FetchContent)

FetchContent_Declare(
  pwropt
  GIT_REPOSITORY https://github.com/BetaJester/pwropt.git
  GIT_TAG main
  GIT_SHALLOW TRUE # get only last version
  GIT_PROGRESS TRUE # show progress of download
  USES_TERMINAL_DOWNLOAD TRUE # show progress in ninja generator
)

FetchContent_MakeAvailable(pwropt)
