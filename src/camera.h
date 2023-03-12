#pragma once
#include "ray.h"
#include "vec3.h"

class Camera {
 public:
  Camera(Point3 lookfrom,
         Point3 lookat,
         Vec3 vup,
         double vertical_fov_deg,
         double aspect_ratio,
         double aperture,
         double focus_dist);
  Ray get_ray(double u, double v) const;

 private:
  Point3 origin;
  Point3 lower_left_corner;
  Vec3 horizontal;
  Vec3 vertical;
  Vec3 u, v, w;
  double lens_radius;
};