add_executable(quaternion_test main.cc)

target_include_directories(quaternion_test
  PRIVATE
  ${doctest_SOURCE_DIR}/doctest
)

target_link_libraries(quaternion_test
  PRIVATE
  quaternion
  m
)

add_test(NAME quaternion-test COMMAND quaternion_test)
