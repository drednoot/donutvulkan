add_library(allocator
  SHARED
  src/arena.cc
)

# add_subdirectory(tests)

target_include_directories(allocator
  PUBLIC
  include
  PRIVATE
  src
)
