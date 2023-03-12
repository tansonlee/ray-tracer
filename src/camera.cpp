#include "camera.h"
#include "helpers.h"

Camera::Camera(Point3 lookfrom,
               Point3 lookat,
               Vec3 vup,
               double vertical_fov_deg,
               double aspect_ratio,
               double aperture,
               double focus_dist) {
  double theta = degrees_to_radians(vertical_fov_deg);
  double h = tan(theta / 2);
  double viewport_height = 2.0 * h;
  double viewport_width = aspect_ratio * viewport_height;

  w = normalize(lookfrom - lookat);
  u = normalize(cross(vup, w));
  v = cross(w, u);

  origin = lookfrom;
  // A vector the length and direction of the width of the viewport.
  horizontal = focus_dist * viewport_width * u;
  // A vector the length and direction of the height of the viewport.
  vertical = focus_dist * viewport_height * v;
  // Lower left corner of the viewport.
  lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

  lens_radius = aperture / 2;
}

Ray Camera::get_ray(double s, double t) const {
  Vec3 rd = lens_radius * random_in_unit_disk();
  Vec3 offset = u * rd.get_x() + v * rd.get_y();
  return Ray(origin + offset, lower_left_corner + s * horizontal +
                                  t * vertical - origin - offset);
}
