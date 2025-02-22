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

  inline Vec3 operator+(const Vec3& other) const {
    return {x + other.x, y + other.y, z + other.z};
  }
  inline Vec3& operator+=(const Vec3& other) {
    *this = *this + other;
    return *this;
  }
  inline Vec3 operator-() const { return Vec3{-x, -y, -z}; }
  inline Vec3 operator-(const Vec3& other) const { return *this + (-other); }
  inline Vec3& operator-=(const Vec3& other) {
    *this = *this - other;
    return *this;
  }

  inline bool IsValid() const {
    return !(std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y) ||
             std::isnan(z) || std::isinf(z));
  }

  inline double Dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
  }
};

}  // namespace quat

#endif  // QUATERNION_VEC3_H_
