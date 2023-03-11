#pragma once

#include "../ray.h"

struct HitRecord;

class Material {
 public:
  Material(){};
  virtual bool scatter(const Ray& ray_in,
                       const HitRecord& hit_record,
                       Color& attenuation,
                       Ray& scattered) const = 0;
  virtual ~Material(){};
};