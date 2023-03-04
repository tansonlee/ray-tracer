#include "camera.h"

Camera::Camera() {
  double aspect_ratio = 16.0 / 9.0;
  double viewport_height = 2.0;
  double viewport_width = aspect_ratio * viewport_height;
  double focal_length = 1.0;

  this->origin = Point3(0, 0, 0);
  // A vector the length and direction of the width of the viewport.
  this->horizontal = Vec3(viewport_width, 0.0, 0.0);
  // A vector the length and direction of the height of the viewport.
  this->vertical = Vec3(0.0, viewport_height, 0.0);
  // Lower left corner of the viewport.
  this->lower_left_corner =
      origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
}

Ray Camera::get_ray(double u, double v) {
  // Ray starts at the origin.
  // To get the end of the ray, take the lower left corner then add the
  // fractional horizontal and vertical components. Subtract the origin from
  // this to get the ray vector.
  return Ray(origin,
             lower_left_corner + u * horizontal + v * vertical - origin);
}
