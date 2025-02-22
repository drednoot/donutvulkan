add_library(core
  SHARED
  src/abstract_renderer.cc
  src/rotation.cc
)

add_subdirectory(tests)

target_include_directories(core
  PUBLIC
  include
)
