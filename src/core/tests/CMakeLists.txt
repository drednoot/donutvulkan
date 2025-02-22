add_executable(core_test main.cc)

target_include_directories(core_test
  PRIVATE
  ${doctest_SOURCE_DIR}/doctest
)

target_link_libraries(core_test
  PRIVATE
  core
  m
)

add_test(NAME core-test COMMAND core_test)
