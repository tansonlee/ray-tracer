#include "sphere.h"
#include <cmath>

Sphere::Sphere(Point3 c, double r, std::shared_ptr<Material> m)
    : center{c}, radius{r}, material{m} {}

bool Sphere::hit(const Ray& r,
                 double t_min,
                 double t_max,
                 HitRecord& rec) const {
  Vec3 oc = r.get_origin() - this->center;
  auto a = r.get_direction().length_squared();
  auto half_b = dot(oc, r.get_direction());
  auto c = oc.length_squared() - radius * radius;

  auto discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;
  auto sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.point = r.at(rec.t);
  rec.normal = (rec.point - center) / radius;
  Vec3 outward_normal = (rec.point - center) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.material = this->material;

  return true;
}