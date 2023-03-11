#pragma once

#include "../vec3.h"
#include "hittable.h"

class Sphere : public Hittable {
 public:
  Sphere(Point3 c, double r, std::shared_ptr<Material> m);
  bool hit(const Ray& r,
           double t_min,
           double t_max,
           HitRecord& rec) const override;

 private:
  Point3 center;
  double radius;
  std::shared_ptr<Material> material;
};