#ifndef DONUTCPP_CORE_QUATERNION_H_
#define DONUTCPP_CORE_QUATERNION_H_

#include <cmath>

#include "vec3.h"

namespace core {

struct Quat {
 public:
  double s;
  double x;
  double y;
  double z;

  inline static Quat FromAxisAndAngle(const Vec3& axis, double theta) {
    const double sin_t_half = sin(theta / 2.0);
    return Quat{
        .s = cos(theta / 2.0),
        .x = sin_t_half * axis.x,
        .y = sin_t_half * axis.y,
        .z = sin_t_half * axis.z,
    };
  }
  inline static Quat Pure(const Vec3& vector_part) {
    return Quat{
        .s = 0.0,
        .x = vector_part.x,
        .y = vector_part.y,
        .z = vector_part.z,
    };
  }

  inline void ToConjugate() { *this = Conjugate(); }
  inline Quat Conjugate() const {
    return Quat{
        .s = s,
        .x = -x,
        .y = -y,
        .z = -z,
    };
  }
  inline Vec3 ExtractVector() const { return Vec3{x, y, z}; }

  inline Quat operator*(const Quat& other) const {
    return Quat{
        .s = s * other.s - x * other.x - y * other.y - z * other.z,
        .x = s * other.x + x * other.s + y * other.z - z * other.y,
        .y = s * other.y - x * other.z + y * other.s + z * other.x,
        .z = s * other.z + x * other.y - y * other.x + z * other.s,
    };
  }
  inline Quat& operator*=(const Quat& other) {
    *this = *this * other;
    return *this;
  }
  inline Quat operator+(const Quat& other) const {
    return Quat{
        .s = s + other.s,
        .x = x + other.x,
        .y = y + other.y,
        .z = z + other.z,
    };
  }
  inline Quat& operator+=(const Quat& other) {
    *this = *this + other;
    return *this;
  }
  inline bool operator==(const Quat& other) const {
    return s == other.s && x == other.x && y == other.y && z == other.z;
  }
  inline bool operator!=(const Quat& other) const { return !(*this == other); }
};

}  // namespace core

#endif  // DONUTCPP_CORE_QUATERNION_H_
