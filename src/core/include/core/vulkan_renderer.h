#ifndef DONUTCPP_CORE_VULKAN_RENDERER_H_
#define DONUTCPP_CORE_VULKAN_RENDERER_H_

#include <chrono>
#include <expected>
#include <memory>

#include "core/result.h"
#include "core/vec3.h"

namespace core {

class VulkanRenderer;

class VulkanRenderHandler {
 public:
  virtual ~VulkanRenderHandler() = default;
  virtual void Render(double delta, VulkanRenderer& renderer) = 0;
};

struct VulkanRendererConfig {
  int width = 0;
  int height = 0;
  int target_fps = 0;
  VulkanRenderHandler* render_handler = nullptr;
};

class VulkanRenderer {
 public:
  static std::expected<VulkanRenderer*, Result> New(
      const VulkanRendererConfig& config);
  ~VulkanRenderer();

  void Start();

  void Clear();
  // puts a char `sym` on the screen at a point (x, y), doesn't check bounds
  void Put(int x, int y, char sym);

  /**
   * puts a char `sym` on the screen at a point `point`, where 0 <= x, y, z < 1
   * checks bounds, checks depth, (probably) slower than Put(int x, int y, char
   * sym)
   */
  void Put(const core::Vec3& point, char sym);

  int GetWidth() const;
  int GetHeight() const;
  double GetRatio() const;

  // converts x and y to buffer index
  int Xy(int x, int y) const;
  // returns rightmost "pixel"
  int Right() const;
  // returns bottommost "pixel"
  int Bot() const;
  double ToAspect(double val) const;
  double NsToSeconds(std::chrono::nanoseconds ns) const;
  double TimeLived() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> d;

  VulkanRenderer();
};

}  // namespace core

#endif  // DONUTCPP_CORE_VULKAN_RENDERER_H_
