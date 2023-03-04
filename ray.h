#pragma once

#include "vec3.h"

class Ray {
 public:
  Ray(const Point3 _origin, const Point3 _direction);

  Point3 get_origin() const;
  Point3 get_direction() const;

  Point3 at(double t) const;

 private:
  Point3 origin;
  Point3 direction;
};