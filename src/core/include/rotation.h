#ifndef DONUTCPP_CORE_ROTATION_H_
#define DONUTCPP_CORE_ROTATION_H_

#include "vec3.h"

namespace core {

/**
 * Rotates point `point` along axis `axis` by an angle `angle`
 *
 * @return rotated point as Vec3
 */
Vec3 Rotate(const Vec3& point, const Vec3& axis, double angle);

}  // namespace core

#endif  // DONUTCPP_CORE_ROTATION_H_
