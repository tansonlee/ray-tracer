#include "material.h"

class Metal : public Material {
 public:
  Metal(const Color& a, double f);

  virtual bool scatter(const Ray& r_in,
                       const HitRecord& rec,
                       Color& attenuation,
                       Ray& scattered) const override;

 public:
  Color albedo;
  double fuzz;
};