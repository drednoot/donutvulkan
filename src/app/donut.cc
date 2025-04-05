#include "donut.h"

#include <numbers>

#include "core/point_info.h"
#include "core/rotation.h"
#include "core/vec3.h"

Donut::Donut(double r1, double r2, int precision) : Object() {
  points_.resize(precision * precision);

  const double angle_step = 2 * std::numbers::pi / precision;
  const core::Vec3 z_axis = {0, 0, 1};
  const core::Vec3 y_axis = {0, 1, 0};
  const core::Vec3 center_at_r1 = {r1, 0, 0};
  const core::Vec3 starting_point{r1 + r2, 0, 0};
  const core::Vec3 normal_starting_point{1, 0, 0};

  for (int major = 0; major < precision; ++major) {
    for (int minor = 0; minor < precision; ++minor) {
      const int ind = major * precision + minor;

      core::Vec3 p = core::Rotate(starting_point, z_axis, angle_step * minor,
                                  center_at_r1);
      p = core::Rotate(p, y_axis, angle_step * major);

      core::Vec3 normal =
          core::Rotate(normal_starting_point, z_axis, angle_step * minor);
      normal = core::Rotate(normal, y_axis, angle_step * major);

      points_[ind] = core::PointInfo{
          .p = p,
          .normal = normal,
      };
    }
  }
}
