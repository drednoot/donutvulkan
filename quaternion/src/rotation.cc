#include "rotation.h"

#include "quaternion.h"
#include "vec3.h"

namespace quat {

Vec3 Rotate(const Vec3& point, const Vec3& axis, double angle) {
  const Vec3 norm_axis = axis.Normalized();
  const Quat q = Quat::FromAxisAndAngle(norm_axis, angle);
  const Quat q_conj = q.Conjugate();
  const Quat h = Quat::Pure(point);

  const Quat rotated_q = q * h * q_conj;
  return rotated_q.ExtractVector();
}

}  // namespace quat
