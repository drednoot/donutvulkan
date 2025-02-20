#ifndef QUATERNION_VEC3_H_
#define QUATERNION_VEC3_H_

#include <cmath>

namespace quat {

struct Vec3 {
 public:
  double x;
  double y;
  double z;

  inline Vec3 Normalized() const {
    double length = sqrt(x * x + y * y + z * z);
    return Vec3{
        .x = x / length,
        .y = y / length,
        .z = z / length,
    };
  };
  inline void Normalize() { *this = Normalized(); }
};

}  // namespace quat

#endif  // QUATERNION_VEC3_H_
