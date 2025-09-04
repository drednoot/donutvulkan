add_executable(allocator_test main.cc)

target_include_directories(allocator_test
  PRIVATE
  ${doctest_SOURCE_DIR}/doctest
)

target_link_libraries(allocator_test
  PRIVATE
  allocator
)

add_test(NAME allocator-test COMMAND allocator_test)
