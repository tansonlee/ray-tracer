#include "lambertian.h"
#include "hittable.h"

Lambertian::Lambertian(const Color& a) : albedo{a} {}

bool Lambertian::scatter(const Ray& r_in,
                         const HitRecord& rec,
                         Color& attenuation,
                         Ray& scattered) const {
  Vec3 scatter_direction = rec.normal + random_unit_vector();

  if (scatter_direction.near_zero()) {
    scatter_direction = rec.normal;
  }

  scattered = Ray(rec.point, scatter_direction);
  attenuation = albedo;
  return true;
}