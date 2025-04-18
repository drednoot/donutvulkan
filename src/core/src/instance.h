#ifndef DONUTCPP_CORE_INSTANCE_H_
#define DONUTCPP_CORE_INSTANCE_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>
#include <vector>

#include "core/result.h"

namespace core {

class Instance {
 public:
  static std::expected<Instance*, Result> New(GLFWwindow* window);

  Instance() = default;
  Instance(const Instance&) = delete;
  Instance(Instance&&) = delete;
  Instance& operator=(const Instance&) = delete;
  Instance& operator=(Instance&&) = delete;
  ~Instance();

  operator const VkInstance&() const { return instance_; }
  const VkSurfaceKHR& GetSurface() const { return surface_; }
#ifndef NDEBUG
  bool IsValidationLayersEnabled() const {
    return is_validation_layers_enabled_;
  }
  static std::expected<std::vector<const char*>, VkResult>
  GetAvailableValidationLayers();
#endif

 private:
  std::expected<VkInstance, VkResult> NewVkInstance();
  std::expected<VkSurfaceKHR, VkResult> NewSurface(GLFWwindow* window) const;

  VkInstance instance_ = VK_NULL_HANDLE;
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
#ifndef NDEBUG
  bool is_validation_layers_enabled_ = false;
#endif
};

}  // namespace core

#endif  // DONUTCPP_CORE_INSTANCE_H_
