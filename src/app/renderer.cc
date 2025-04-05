#include "renderer.h"

#include <algorithm>
#include <cmath>
#include <expected>
#include <memory>

#include "core/point_info.h"
#include "core/result.h"
#include "core/rotation.h"
#include "core/vec3.h"
#include "core/vulkan_renderer.h"

#include "config.h"

std::expected<Renderer*, core::Result> Renderer::New() {
  std::unique_ptr<Renderer> rend(new Renderer);

  core::VulkanRendererConfig config{
      .width = config::kWindowWidth,
      .height = config::kWindowHeight,
      .target_fps = config::kTargetFps,
      .render_handler = rend.get(),
  };

  rend->renderer_.reset(TRY(core::VulkanRenderer::New(config)));
  rend->donut_ = Donut(config::kDonutMajorR, config::kDonutMinorR,
                       config::kDonutPrecision);
  rend->angle_ = 0.0;

  return rend.release();
}

void Renderer::Start() {
  renderer_->Start();
}

void Renderer::Render(double delta, core::VulkanRenderer& renderer) {
  angle_ += 2.5 * delta;

  for (const core::PointInfo& pi : donut_.Points()) {
    const core::Vec3 rotate_axis1{0.1, 0.2, 0.5};
    const core::Vec3 rotate_axis2{0.7, 0.7, -0.5};
    core::Vec3 p = pi.p;

    p = core::Rotate(p, rotate_axis1, angle_);
    p = core::Rotate(p, rotate_axis2, angle_ * 0.2);
    p += core::Vec3{0.5, 0.5, 0.5};  // move it to the center of the screen
    p.x /= renderer.Ratio();

    core::Vec3 normal = pi.normal;
    normal = core::Rotate(normal, rotate_axis1, angle_);
    normal = core::Rotate(normal, rotate_axis2, angle_ * 0.2);

    double dot = std::max(config::kLightPoint.Dot(normal),
                          0.0);  // clamp to zero if negative
    dot = std::min(dot, 1.0);
    int light_index = (int)(dot * config::kLightLevelCount);
    if (light_index > config::kLightLevelCount - 1) {
      light_index = config::kLightLevelCount - 1;
    }

    char c = config::kLightLevles[light_index];

    renderer.Put(p, c);
  }
}
