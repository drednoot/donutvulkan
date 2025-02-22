#include "renderer.h"

#include <cmath>
#include <numbers>
#include <string>

#include "abstract_renderer.h"
#include "config.h"
#include "rotation.h"
#include "vec3.h"

Renderer::Renderer() : AbstractRenderer(), angle_(0.0) {
  // generate cube points
  const int cubesize = config::kCubeSidePresicion;
  const double step = config::kCubeSideSize / cubesize;

  struct Transition {
    quat::Vec3 move;
    quat::Vec3 rotate;
    quat::Vec3 normal;
    double angle;
  };

  Transition face_transitions[6] = {
      Transition{
          .move = {0.0, 0.0, 0.0},
          .rotate = {0.0, 0.0, 0.0},
          .normal = {0.0, 0.0, -1.0},
          .angle = 0.0,
      },
      Transition{
          .move = {0.0, 0.0, 0.0},
          .rotate = {0.0, 1.0, 0.0},
          .normal = {-1.0, 0.0, 0.0},
          .angle = -std::numbers::pi / 2.0,
      },
      Transition{
          .move = {0.0, 0.0, 0.0},
          .rotate = {1.0, 0.0, 0.0},
          .normal = {0.0, -1.0, 0.0},
          .angle = std::numbers::pi / 2.0,
      },
      Transition{
          .move = {0.0, 0.0, config::kCubeSideSize},
          .rotate = {0.0, 0.0, 0.0},
          .normal = {0.0, 0.0, 1.0},
          .angle = 0.0,
      },
      Transition{
          .move = {config::kCubeSideSize, 0.0, 0.0},
          .rotate = {0.0, 1.0, 0.0},
          .normal = {1.0, 0.0, 0.0},
          .angle = -std::numbers::pi / 2.0,
      },
      Transition{
          .move = {0.0, config::kCubeSideSize, 0.0},
          .rotate = {1.0, 0.0, 0.0},
          .normal = {0.0, 1.0, 0.0},
          .angle = std::numbers::pi / 2.0,
      },
  };

  for (int side = 0; side < 6; ++side) {
    const Transition& cur_trans = face_transitions[side];
    const quat::Vec3& axis = cur_trans.rotate;
    const quat::Vec3& move = cur_trans.move;
    const quat::Vec3& normal = cur_trans.normal;

    for (int i = 0; i < cubesize; ++i) {
      for (int j = 0; j < cubesize; ++j) {
        const int ind = side * cubesize * cubesize + j * cubesize + i;

        double x = step * i;
        double y = step * j;

        quat::Vec3 out =
            quat::Rotate(quat::Vec3{x, y, 0.0}, axis, cur_trans.angle);
        out += move;

        cube_points_[ind] = PointInfo{.p = out, .normal = normal};
      }
    }
  }
}

void Renderer::Render(double delta) {
  const double time_lived = TimeLived();
  angle_ += 2.5 * delta;

  for (const PointInfo& pi : cube_points_) {
    const quat::Vec3 rotate_axis{0.5, 0.5, 0.5};

    quat::Vec3 out = quat::Rotate(pi.p, rotate_axis, angle_);
    const quat::Vec3& out_normal = quat::Rotate(pi.normal, rotate_axis, angle_);
    out.x += 0.5;
    out.y += 0.25;
    out.x /= Ratio();

    double dot = config::kLightPoint.Dot(out_normal);
    int light_index = (int)(dot * config::kLightLevelCount);
    if (light_index > config::kLightLevelCount - 1) {
      light_index = config::kLightLevelCount - 1;
    }

    char c = config::kLightLevles[light_index];

    Put(out.x, out.y, c);
  }

  std::string dfs = std::to_string(time_lived);
  for (int i = 0, size = dfs.size(); i < size; ++i) {
    Put(i, Bot(), dfs[i]);
  }
}
