#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>

#include <sys/mman.h>
#include <unistd.h>

#include "allocator/arena.h"

using namespace allocator;

inline const auto kPageSize = sysconf(_SC_PAGESIZE);

auto Align(void* ptr) -> void* {
  return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) &
                                 ~(kPageSize - 1));
}

TEST_CASE("Simple allocation") {
  Arena a;

  unsigned char* vec =
      reinterpret_cast<unsigned char*>(a.Alloc(sizeof(unsigned char)));

  CHECK_EQ(mincore(Align(vec), kPageSize, vec), 0);
}

TEST_CASE("Typed allocation") {
  Arena a;

  auto* vec = a.Alloc<unsigned char>();

  CHECK_EQ(mincore(Align(vec), kPageSize, vec), 0);
}

TEST_CASE("Scoped deallocation") {
  unsigned char vec;
  int* p;

  {
    Arena a;

    p = a.Alloc<int>();

    CHECK_EQ(mincore(Align(p), kPageSize, &vec), 0);
  }

  CHECK_EQ(mincore(Align(p), kPageSize, &vec), -1);
  CHECK_EQ(errno, ENOMEM);
}

TEST_CASE("Allocating a page worth") {
  Arena a;
  constexpr const int kExpectedPages = 2;
  unsigned char vec[kExpectedPages];

  auto* p = a.Alloc(kPageSize);

  CHECK_EQ(mincore(Align(p), kPageSize * kExpectedPages, vec), 0);
}

TEST_CASE("Allocating half a page and then entire page") {
  Arena a;
  const int kExpectedPages1 = 1;
  const int kExpectedPages2 = 2;
  unsigned char vec1[kExpectedPages1];
  unsigned char vec2[kExpectedPages2];

  auto* p1 = a.Alloc(kPageSize / 2);
  auto* p2 = a.Alloc(kPageSize);

  CHECK_EQ(mincore(Align(p1), kPageSize * kExpectedPages1, vec1), 0);
  CHECK_EQ(mincore(Align(p2), kPageSize * kExpectedPages2, vec2), 0);
}
