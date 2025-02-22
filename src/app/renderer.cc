#include "renderer.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <string>

#include "abstract_renderer.h"
#include "config.h"
#include "point_info.h"
#include "rotation.h"
#include "vec3.h"

Renderer::Renderer()
    : AbstractRenderer(config::kTargetFps, config::kSceneLiveTimeSeconds),
      cube_(config::kCubeSideSize, config::kCubeSidePresicion),
      angle_(0.0) {}

void Renderer::Render(double delta) {
  const double time_lived = TimeLived();
  angle_ += 2.5 * delta;

  for (const core::PointInfo& pi : cube_.Points()) {
    const core::Vec3 rotate_axis{0.1, 0.2, -0.5};
    core::Vec3 p = pi.p;

    p = core::Rotate(p, rotate_axis, angle_, {0.25, 0.25, 0.25});
    p += core::Vec3{0.25, 0.25, 0.25};  // move it to the center of the screen
    p.x /= Ratio();

    const core::Vec3& out_normal = core::Rotate(pi.normal, rotate_axis, angle_);

    double dot = std::max(config::kLightPoint.Dot(out_normal),
                          0.0);  // clamp to zero if negative
    int light_index = (int)(dot * config::kLightLevelCount);
    if (light_index > config::kLightLevelCount - 1) {
      light_index = config::kLightLevelCount - 1;
    }

    char c = config::kLightLevles[light_index];

    Put(p, c);
  }

  std::string dfs = std::to_string(time_lived);
  for (int i = 0, size = dfs.size(); i < size; ++i) {
    Put(i, Bot(), dfs[i]);
  }
}
