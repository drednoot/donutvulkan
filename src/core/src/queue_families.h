#ifndef DONUTCPP_CORE_QUEUE_FAMILIES_H_
#define DONUTCPP_CORE_QUEUE_FAMILIES_H_

#include <cstdint>
#include <optional>

namespace core {

struct QueueFamilies {
  std::optional<uint32_t> graphicsFamily;
};

}  // namespace core

#endif  // DONUTCPP_CORE_QUEUE_FAMILIES_H_
