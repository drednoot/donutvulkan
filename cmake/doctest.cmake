include(FetchContent)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG dev
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(doctest)
