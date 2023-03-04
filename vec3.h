#pragma once
#include <iostream>

class Vec3 {
 public:
  Vec3();
  Vec3(double _x, double _y, double _z);

  double get_x() const;
  double get_y() const;
  double get_z() const;

  double length() const;
  double length_squared() const;

  double operator[](int i) const;
  Vec3 operator-() const;
  Vec3& operator+=(const Vec3& v);
  Vec3& operator*=(const double t);
  Vec3& operator/=(const double t);

 private:
  double x, y, z;
};

std::ostream& operator<<(std::ostream& out, const Vec3& v);

Vec3 operator+(const Vec3& u, const Vec3& v);
Vec3 operator-(const Vec3& u, const Vec3& v);
Vec3 operator*(const Vec3& u, const Vec3& v);
Vec3 operator*(double t, const Vec3& v);
Vec3 operator*(const Vec3& v, double t);
Vec3 operator/(Vec3 v, double t);

double dot(const Vec3& u, const Vec3& v);
Vec3 cross(const Vec3& u, const Vec3& v);
Vec3 normalize(Vec3 v);

Vec3 random_vec3();
Vec3 random_vec3(double min, double max);
Vec3 random_in_unit_sphere();
Vec3 random_unit_vector();
Vec3 random_in_hemisphere(const Vec3& normal);

using Point3 = Vec3;
using Color = Vec3;
