#include "vec3.h"
#include "helpers.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

Vec3::Vec3() : x{0}, y{0}, z{0} {}
Vec3::Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

double Vec3::get_x() const {
  return this->x;
}

double Vec3::get_y() const {
  return this->y;
}

double Vec3::get_z() const {
  return this->z;
}

double Vec3::operator[](int i) const {
  switch (i) {
    case 0:
      return this->x;
    case 1:
      return this->y;
    case 2:
      return this->z;
    default:
      throw std::invalid_argument(
          "Invalid argument to double Vec3::operator[](int i) const: " +
          std::to_string(i));
  }
}

Vec3 Vec3::operator-() const {
  return Vec3(-this->x, -this->y, -this->z);
}

Vec3& Vec3::operator+=(const Vec3& v) {
  this->x += v.get_x();
  this->y += v.get_y();
  this->z += v.get_z();

  return *this;
}

Vec3& Vec3::operator*=(const double k) {
  this->x *= k;
  this->y *= k;
  this->z *= k;

  return *this;
}

Vec3& Vec3::operator/=(const double k) {
  this->x /= k;
  this->y /= k;
  this->z /= k;

  return *this;
}

double Vec3::length_squared() const {
  return this->x * this->x + this->y * this->y + this->z * this->z;
}

double Vec3::length() const {
  return sqrt(this->length_squared());
}

bool Vec3::near_zero() const {
  const double epsilon = 1e-8;
  return (fabs(x) < epsilon) && (fabs(y) < epsilon) && (fabs(z) < epsilon);
}

std::ostream& operator<<(std::ostream& out, const Vec3& v) {
  return out << v.get_x() << " " << v.get_y() << " " << v.get_z() << std::endl;
}

Vec3 operator+(const Vec3& u, const Vec3& v) {
  return Vec3(u.get_x() + v.get_x(), u.get_y() + v.get_y(),
              u.get_z() + v.get_z());
}

Vec3 operator-(const Vec3& u, const Vec3& v) {
  return Vec3(u.get_x() - v.get_x(), u.get_y() - v.get_y(),
              u.get_z() - v.get_z());
}

Vec3 operator*(const Vec3& u, const Vec3& v) {
  return Vec3(u.get_x() * v.get_x(), u.get_y() * v.get_y(),
              u.get_z() * v.get_z());
}

Vec3 operator*(double t, const Vec3& v) {
  return Vec3(t * v.get_x(), t * v.get_y(), t * v.get_z());
}

Vec3 operator*(const Vec3& v, double t) {
  return t * v;
}

Vec3 operator/(Vec3 v, double t) {
  return (1 / t) * v;
}

double dot(const Vec3& u, const Vec3& v) {
  return u.get_x() * v.get_x() + u.get_y() * v.get_y() + u.get_z() * v.get_z();
}

Vec3 cross(const Vec3& u, const Vec3& v) {
  return Vec3(u.get_y() * v.get_z() - u.get_z() * v.get_y(),
              u.get_z() * v.get_x() - u.get_x() * v.get_z(),
              u.get_x() * v.get_y() - u.get_y() * v.get_x());
}

Vec3 normalize(Vec3 v) {
  if (v.length() == 0) {
    std::cerr << "Normalizing zero vector" << std::endl;
  }
  return v / v.length();
}

Vec3 random_vec3() {
  return Vec3{random_double(), random_double(), random_double()};
}

Vec3 random_vec3(double min, double max) {
  return Vec3{random_double(min, max), random_double(min, max),
              random_double(min, max)};
}

Vec3 random_in_unit_sphere() {
  while (true) {
    Vec3 candidate = random_vec3(-1, 1);
    if (candidate.length_squared() < 1) {
      return candidate;
    }
  }
}

Vec3 random_unit_vector() {
  return normalize(random_in_unit_sphere());
}

Vec3 random_in_hemisphere(const Vec3& normal) {
  Vec3 in_unit_sphere = random_in_unit_sphere();
  if (dot(in_unit_sphere, normal) > 0.0) {
    return in_unit_sphere;
  } else {
    return -in_unit_sphere;
  }
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
  return v - 2 * dot(v, n) * n;
}

Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
  auto cos_theta = fmin(dot(-uv, n), 1.0);
  Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}