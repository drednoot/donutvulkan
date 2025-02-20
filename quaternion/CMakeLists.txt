add_library(quaternion
  STATIC
  src/rotation.cc
)

target_include_directories(quaternion
  PUBLIC
  src
)

target_link_libraries(quaternion PRIVATE m)

add_subdirectory(tests)
