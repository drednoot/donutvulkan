#ifndef DONUTVULKAN_ALLOCATOR_ARENA_H_
#define DONUTVULKAN_ALLOCATOR_ARENA_H_

#include <cstdint>

namespace allocator {

class Arena {
 public:
  Arena();
  ~Arena();

  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;
  Arena(Arena&&) = default;
  Arena& operator=(Arena&&) = default;

  template <typename T>
  auto Alloc() -> T* {
    return Alloc(sizeof(T));
  }
  auto Alloc(uint32_t size) -> void*;

 private:
  struct Block {
    void* start = nullptr;
    Block* next = nullptr;
    uint32_t page_count;
  };

  auto AllocatePages(uint32_t count) -> void;
  auto LastBlockCapacity() const -> uint32_t;

  auto PageSize() const -> uint32_t;
  auto AllocCommand() const -> void*;

  Block* start_ = nullptr;
  Block* last_block_ = nullptr;
  uint32_t size_ = 0;
};

}  // namespace allocator

#endif  // DONUTVULKAN_ALLOCATOR_ARENA_H_
