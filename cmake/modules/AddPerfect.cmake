include_guard()

include(FetchContent)

FetchContent_Declare(
  perfect
  GIT_REPOSITORY https://github.com/abduld/perfect.git
  GIT_TAG master
  GIT_SHALLOW TRUE # get only last version
  GIT_PROGRESS TRUE # show progress of download
  USES_TERMINAL_DOWNLOAD TRUE # show progress in ninja generator
)

FetchContent_MakeAvailable(perfect)
