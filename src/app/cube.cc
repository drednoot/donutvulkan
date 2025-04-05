#include "cube.h"

#include <numbers>

#include "core/point_info.h"
#include "core/rotation.h"
#include "core/vec3.h"

Cube::Cube(double side_size, int precision)
    : Object(), side_size_(side_size), precision_(precision) {
  points_.resize(precision * precision * 6);
  const double step = side_size_ / precision_;

  struct Transition {
    core::Vec3 move;
    core::Vec3 rotate;
    core::Vec3 normal;
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
          .move = {0.0, 0.0, side_size_},
          .rotate = {0.0, 0.0, 0.0},
          .normal = {0.0, 0.0, 1.0},
          .angle = 0.0,
      },
      Transition{
          .move = {side_size_, 0.0, 0.0},
          .rotate = {0.0, 1.0, 0.0},
          .normal = {1.0, 0.0, 0.0},
          .angle = -std::numbers::pi / 2.0,
      },
      Transition{
          .move = {0.0, side_size_, 0.0},
          .rotate = {1.0, 0.0, 0.0},
          .normal = {0.0, 1.0, 0.0},
          .angle = std::numbers::pi / 2.0,
      },
  };

  for (int side = 0; side < 6; ++side) {
    const Transition& cur_trans = face_transitions[side];
    const core::Vec3& axis = cur_trans.rotate;
    const core::Vec3& move = cur_trans.move;
    const core::Vec3& normal = cur_trans.normal;

    for (int i = 0; i < precision_; ++i) {
      for (int j = 0; j < precision_; ++j) {
        const int ind = side * precision_ * precision_ + j * precision_ + i;

        double x = step * i;
        double y = step * j;

        core::Vec3 out =
            core::Rotate(core::Vec3{x, y, 0.0}, axis, cur_trans.angle);
        out += move;

        points_[ind] = core::PointInfo{.p = out, .normal = normal};
      }
    }
  }
}
