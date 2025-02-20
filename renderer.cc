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
    double move_x;
    double move_y;
    double move_z;
    double rotate_x;
    double rotate_y;
    double rotate_z;
    double angle;
  };

  Transition face_transitions[6] = {
      Transition{
          .move_x = 0.0,
          .move_y = 0.0,
          .move_z = 0.0,
          .rotate_x = 0.0,
          .rotate_y = 0.0,
          .rotate_z = 0.0,
          .angle = 0.0,
      },
      Transition{
          .move_x = 0.0,
          .move_y = 0.0,
          .move_z = 0.0,
          .rotate_x = 0.0,
          .rotate_y = 1.0,
          .rotate_z = 0.0,
          .angle = -std::numbers::pi / 2.0,
      },
      Transition{
          .move_x = 0.0,
          .move_y = 0.0,
          .move_z = 0.0,
          .rotate_x = 1.0,
          .rotate_y = 0.0,
          .rotate_z = 0.0,
          .angle = std::numbers::pi / 2.0,
      },
      Transition{
          .move_x = 0.0,
          .move_y = 0.0,
          .move_z = config::kCubeSideSize,
          .rotate_x = 0.0,
          .rotate_y = 0.0,
          .rotate_z = 0.0,
          .angle = 0.0,
      },
      Transition{
          .move_x = config::kCubeSideSize,
          .move_y = 0.0,
          .move_z = 0.0,
          .rotate_x = 0.0,
          .rotate_y = 1.0,
          .rotate_z = 0.0,
          .angle = -std::numbers::pi / 2.0,
      },
      Transition{
          .move_x = 0.0,
          .move_y = config::kCubeSideSize,
          .move_z = 0.0,
          .rotate_x = 1.0,
          .rotate_y = 0.0,
          .rotate_z = 0.0,
          .angle = std::numbers::pi / 2.0,
      },
  };

  const char alpha[] = "123456";

  for (int side = 0; side < 6; ++side) {
    const Transition& cur_trans = face_transitions[side];
    const quat::Vec3 axis =
        quat::Vec3{cur_trans.rotate_x, cur_trans.rotate_y, cur_trans.rotate_z};
    const quat::Vec3 move =
        quat::Vec3{cur_trans.move_x, cur_trans.move_y, cur_trans.move_z};

    for (int i = 0; i < cubesize; ++i) {
      for (int j = 0; j < cubesize; ++j) {
        const int ind = side * cubesize * cubesize + j * cubesize + i;

        double x = step * i;
        double y = step * j;

        quat::Vec3 out =
            quat::Rotate(quat::Vec3{x, y, 0.0}, axis, cur_trans.angle);
        out += move;

        cube_points_[ind] = PointInfo{.p = out, .c = alpha[side]};
      }
    }
  }
}

void Renderer::Render(double delta) {
  const double time_lived = TimeLived();
  angle_ += 2.5 * delta;

  for (const PointInfo& pi : cube_points_) {
    quat::Vec3 out = quat::Rotate(pi.p, quat::Vec3{0.5, 0.5, 0.5}, angle_);
    out.x += 0.5;
    out.x /= Ratio();
    Put(out.x, out.y, pi.c);
  }

  std::string dfs = std::to_string(time_lived);
  for (int i = 0, size = dfs.size(); i < size; ++i) {
    Put(i, Bot(), dfs[i]);
  }
}
