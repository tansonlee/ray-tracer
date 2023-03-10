#include "metal.h"
#include "../hittable.h"
#include "../ray.h"

Metal::Metal(const Color& a, double f) : albedo{a}, fuzz{f} {}

bool Metal::scatter(const Ray& r_in,
                    const HitRecord& rec,
                    Color& attenuation,
                    Ray& scattered) const {
  Vec3 reflected = reflect(normalize(r_in.get_direction()), rec.normal);
  scattered = Ray(rec.point, reflected + fuzz * random_in_unit_sphere());
  attenuation = albedo;
  return (dot(scattered.get_direction(), rec.normal) > 0);
}