#include "ray.h"
#include "vec3.h"

Ray::Ray() {}

Ray::Ray(const Point3 _origin, const Point3 _direction)
    : origin{_origin}, direction{_direction} {}

Point3 Ray::get_origin() const {
  return this->origin;
}

Point3 Ray::get_direction() const {
  return this->direction;
}

Point3 Ray::at(double t) const {
  return this->origin + t * this->direction;
}