#ifndef DONUTCPP_CORE_VULKAN_RENDERER_H_
#define DONUTCPP_CORE_VULKAN_RENDERER_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>
#include <expected>
#include <vector>

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
  int width;
  int height;
  int target_fps;
  VulkanRenderHandler* render_handler;
};

class VulkanRenderer {
 public:
  static std::expected<VulkanRenderer*, Result> New(
      const VulkanRendererConfig& config);
  ~VulkanRenderer();

  void Start();

  void Clear();
  // puts a char `sym` on the screen at a point (x, y), doesn't check bounds
  inline void Put(int x, int y, char sym) { buffer_[Xy(x, y)] = sym; }

  /**
   * puts a char `sym` on the screen at a point `point`, where 0 <= x, y, z < 1
   * checks bounds, checks depth, (probably) slower than Put(int x, int y, char
   * sym)
   */
  void Put(const core::Vec3& point, char sym);

  int Width() const { return cfg_.width; };
  int Height() const { return cfg_.height; };
  double Ratio() const { return screen_ratio_; };

  // converts x and y to buffer index
  inline int Xy(int x, int y) const { return y * cfg_.width + x; }
  // returns rightmost "pixel"
  inline int Right() const { return cfg_.width - 1; }
  // returns bottommost "pixel"
  inline int Bot() const { return cfg_.height - 1; }
  inline double ToAspect(double val) const { return val * Ratio(); }
  inline double NsToSeconds(std::chrono::nanoseconds ns) const {
    const long count =
        std::chrono::duration_cast<std::chrono::milliseconds, long>(ns).count();
    return count / 1000.0;
  }
  inline double TimeLived() const {
    std::chrono::nanoseconds now =
        std::chrono::system_clock::now().time_since_epoch();
    std::chrono::nanoseconds from_start = now - start_time_;
    return NsToSeconds(from_start);
  }

 private:
  VulkanRenderer() {}

  // vulkan stuff
  static std::expected<VkInstance, VkResult> CreateVkInstance();

  void DrawBuffer() const;

  void MoveCursorTo(int x, int y) const;

  VulkanRendererConfig cfg_;

  GLFWwindow* window_;
  VkInstance instance_;

  std::vector<char> buffer_;
  std::vector<double> z_buffer_;
  double screen_ratio_;

  std::chrono::nanoseconds target_ns_;
  std::chrono::nanoseconds start_time_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_VULKAN_RENDERER_H_
