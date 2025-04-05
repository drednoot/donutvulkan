#ifndef DONUTCPP_APP_RENDERER_H_
#define DONUTCPP_APP_RENDERER_H_

#include <expected>
#include <memory>

#include "core/result.h"
#include "core/vulkan_renderer.h"
#include "donut.h"

class Renderer : core::VulkanRenderHandler {
 public:
  virtual ~Renderer() = default;
  static std::expected<Renderer*, core::Result> New();

  void Start();

 protected:
  void Render(double delta, core::VulkanRenderer& renderer) override;

 private:
  Renderer() {};

  std::unique_ptr<core::VulkanRenderer> renderer_;
  Donut donut_;
  double angle_;
};

#endif  // DONUTCPP_APP_RENDERER_H_
