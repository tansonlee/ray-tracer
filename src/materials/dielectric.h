#include "material.h"

class Dielectric : public Material {
 public:
  Dielectric(double ior);

  virtual bool scatter(const Ray& r_in,
                       const HitRecord& rec,
                       Color& attenuation,
                       Ray& scattered) const override;

 private:
  double index_of_refraction;
  static double reflectance(double cosine, double ref_idx);
};