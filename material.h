#pragma once

#include "hittable.h"

class Material {
 public:
  virtual bool scatter(const Ray& ray_in,
                       const HitRecord& hit_record,
                       Color& attenuation,
                       Ray& scattered) const = 0;
};