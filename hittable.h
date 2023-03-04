#pragma once

#include "ray.h"
#include "vec3.h"

struct HitRecord {
  // Point where the hit happens.
  Point3 point;
  // Surface normal for the position hit.
  Vec3 normal;
  // Point along the ray that has a hit.
  double t;
  // Whether or not we hit the outwards pointing face.
  bool front_face;

  HitRecord() {}

  void set_face_normal(const Ray& r, const Vec3& outward_normal) {
    front_face = dot(r.get_direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
 public:
  virtual bool hit(const Ray& r,
                   double t_min,
                   double t_max,
                   HitRecord& rec) const = 0;
  virtual ~Hittable(){};
};