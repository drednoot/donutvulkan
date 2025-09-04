#include "allocator/arena.h"

#include <sys/mman.h>
#include <unistd.h>
#include <cassert>
#include <cstdint>
#include <new>

namespace allocator {

Arena::Arena() {}

Arena::~Arena() {
  Block* cur_block = start_;
  const uint32_t page_size = PageSize();

  while (cur_block->next) {
    Block* const prev_block = cur_block;
    cur_block = cur_block->next;
    munmap(prev_block, prev_block->page_count * page_size);
  }
  munmap(cur_block, cur_block->page_count * page_size);
}

auto Arena::Alloc(uint32_t size) -> void* {
  if (size == 0) {
    return nullptr;
  }

  if (LastBlockCapacity() - size_ >= size) {
    if (!last_block_) {
      AllocatePages(1);
    }
    void* const location =
        reinterpret_cast<uint8_t*>(last_block_->start) + size_;
    size_ += size;
    return location;
  } else {
    const uint32_t page_size = PageSize();
    const uint32_t needed_pages_raw = size / page_size;
    const uint32_t needed_pages =
        needed_pages_raw * page_size - sizeof(Block) < size
            ? needed_pages_raw + 1
            : needed_pages_raw;

    AllocatePages(needed_pages);
    size_ = size;
    return last_block_->start;
  }
}

auto Arena::AllocatePages(uint32_t count) -> void {
  if (count == 0) {
    return;
  }

  const uint32_t page_size = PageSize();
  void* addr =
      last_block_ ? last_block_ + page_size * last_block_->page_count : nullptr;
  void* const block_start =
      mmap(addr, page_size * count, PROT_READ | PROT_WRITE,
           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (block_start == MAP_FAILED) {
    throw std::bad_alloc();
  }

  *reinterpret_cast<Block*>(block_start) = Block{
      .start = reinterpret_cast<Block*>(block_start) + 1,
      .next = nullptr,
      .page_count = count,
  };

  if (last_block_) {
    last_block_->next = reinterpret_cast<Block*>(block_start);
  } else {
    start_ = last_block_;
  }
  last_block_ = reinterpret_cast<Block*>(block_start);
  size_ = 0;
}

auto Arena::PageSize() const -> uint32_t {
  return sysconf(_SC_PAGESIZE);
}

auto Arena::LastBlockCapacity() const -> uint32_t {
  const auto page_size = PageSize();
  return last_block_ ? page_size * last_block_->page_count - sizeof(Block)
                     : page_size - sizeof(Block);
}

}  // namespace allocator
