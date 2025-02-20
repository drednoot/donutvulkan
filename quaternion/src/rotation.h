#ifndef QUATERNION_ROTATION_H_
#define QUATERNION_ROTATION_H_

#include "vec3.h"

namespace quat {

/**
 * Rotates point `point` along axis `axis` by an angle `angle`
 *
 * @return rotated point as Vec3
 */
Vec3 Rotate(const Vec3& point, const Vec3& axis, double angle);

}  // namespace quat

#endif  // QUATERNION_ROTATION_H_
