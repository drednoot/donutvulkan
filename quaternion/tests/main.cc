#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>
#include <cmath>
#include <numbers>

#include "quaternion.h"
#include "rotation.h"
#include "vec3.h"

using namespace std::numbers;
using namespace quat;

TEST_CASE("From Axis And Angle") {
  const Vec3 axis{1.0, 0.0, 0.0};
  const Quat q = Quat::FromAxisAndAngle(axis, pi / 2.0);

  CHECK_EQ(q.s, doctest::Approx(cos(pi / 4.0)));
  CHECK_EQ(q.x, doctest::Approx(sin(pi / 4.0)));
  CHECK_EQ(q.y, doctest::Approx(0.0));
  CHECK_EQ(q.z, doctest::Approx(0.0));
}

TEST_CASE("Pure") {
  const Vec3 vector_part{2.2, 8.7, 7.7};
  const Quat q = Quat::Pure(vector_part);

  CHECK_EQ(q.s, doctest::Approx(0.0));
  CHECK_EQ(q.x, doctest::Approx(vector_part.x));
  CHECK_EQ(q.y, doctest::Approx(vector_part.y));
  CHECK_EQ(q.z, doctest::Approx(vector_part.z));
}

TEST_CASE("ToConjugate") {
  Quat q = {1.0, 3.0, 3.0, 7.0};
  q.ToConjugate();

  CHECK_EQ(q.s, doctest::Approx(1.0));
  CHECK_EQ(q.x, doctest::Approx(-3.0));
  CHECK_EQ(q.y, doctest::Approx(-3.0));
  CHECK_EQ(q.z, doctest::Approx(-7.0));
}

TEST_CASE("Conjugate") {
  const Quat q = {1.0, 3.0, 3.0, 7.0};
  const Quat q_conj = q.Conjugate();

  CHECK_EQ(q_conj.s, doctest::Approx(1.0));
  CHECK_EQ(q_conj.x, doctest::Approx(-3.0));
  CHECK_EQ(q_conj.y, doctest::Approx(-3.0));
  CHECK_EQ(q_conj.z, doctest::Approx(-7.0));
}

TEST_CASE("Extract Vector") {
  const Quat q = {1.0, 3.0, 3.0, 7.0};
  const Vec3 vec = q.ExtractVector();

  CHECK_EQ(q.x, doctest::Approx(vec.x));
  CHECK_EQ(q.y, doctest::Approx(vec.y));
  CHECK_EQ(q.z, doctest::Approx(vec.z));
}

TEST_CASE("Mult q*q") {
  Quat q1 = {-9.0, 2.0, -2.0, 6.0};
  const Quat q2 = {-5.0, 6.0, 8.0, 0.0};

  const Quat result = q1 * q2;

  CHECK_EQ(result.s, doctest::Approx(49.0));
  CHECK_EQ(result.x, doctest::Approx(-112.0));
  CHECK_EQ(result.y, doctest::Approx(-26.0));
  CHECK_EQ(result.z, doctest::Approx(-2.0));

  q1 *= q2;

  CHECK(q1 == result);
}

TEST_CASE("Add q+q") {
  Quat q1 = {1.0, 3.0, 3.0, 7.0};
  const Quat q2 = {3.0, 9.0, 3.0, 9.0};

  const Quat result = q1 + q2;

  CHECK_EQ(result.s, doctest::Approx(4.0));
  CHECK_EQ(result.x, doctest::Approx(12.0));
  CHECK_EQ(result.y, doctest::Approx(6.0));
  CHECK_EQ(result.z, doctest::Approx(16.0));

  q1 += q2;

  CHECK(q1 == result);
}

TEST_CASE("Normalize (const method) Vec3") {
  const Vec3 v{3.0, 3.0, 3.0};

  const Vec3 norm = v.Normalized();

  CHECK_EQ(norm.x, doctest::Approx(doctest::Approx(0.57735)));
  CHECK_EQ(norm.y, doctest::Approx(doctest::Approx(0.57735)));
  CHECK_EQ(norm.z, doctest::Approx(doctest::Approx(0.57735)));
}

TEST_CASE("Normalized (non-const method) Vec3") {
  const Vec3 v{3.0, 4.0, 5.0};

  const Vec3 norm = v.Normalized();

  CHECK_EQ(norm.x, doctest::Approx(0.424263));
  CHECK_EQ(norm.y, doctest::Approx(0.565684));
  CHECK_EQ(norm.z, doctest::Approx(0.707107));
}

TEST_CASE("Rotate Simple") {
  const Vec3 point{2.0, 1.0, 3.0};
  const Vec3 x_axis{1.0, 0.0, 0.0};
  const double deg_90 = pi / 2.0;

  const Vec3 rotated = Rotate(point, x_axis, deg_90);

  CHECK_EQ(rotated.x, doctest::Approx(2.0));
  CHECK_EQ(rotated.y, doctest::Approx(-3.0));
  CHECK_EQ(rotated.z, doctest::Approx(1.0));
}
