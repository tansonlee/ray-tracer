#include <cmath>
#include <iostream>
#include <memory>
#include "camera.h"
#include "color.h"
#include "helpers.h"
#include "hittables/hittable_list.h"
#include "hittables/sphere.h"
#include "materials/dielectric.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "ray.h"
#include "vec3.h"

int MAX_DEPTH = 50;

double map(double x,
           double in_min,
           double in_max,
           double out_min,
           double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Color lerp_color(Color color1, Color color2, double t) {
  return (1.0 - t) * color1 + t * color2;
}

// Returns where along the ray hits the sphere. -1 if it does not hit the
// sphere.
double hit_sphere(const Point3& center, double radius, const Ray& r) {
  Vec3 oc = r.get_origin() - center;
  auto a = r.get_direction().length_squared();
  auto half_b = dot(oc, r.get_direction());
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-half_b - sqrt(discriminant)) / a;
  }
}

Color ray_color(Ray r, HittableList world, int depth) {
  if (depth > MAX_DEPTH) {
    return Color{0, 0, 0};
  }

  HitRecord record;
  if (world.hit(r, 0.001, infinity, record)) {
    Ray scattered;
    Color attenuation;
    if (record.material->scatter(r, record, attenuation, scattered)) {
      return attenuation * ray_color(scattered, world, depth + 1);
    }
    return Color{0, 0, 0};
  }

  Vec3 unit_direction = normalize(r.get_direction());
  const double t = map(unit_direction.get_y(), -1, 1, 0, 1);
  return lerp_color(Color{1.0, 1.0, 1.0}, Color{0.5, 0.7, 1.0}, t);
}

int main() {
  const int samples_per_pixel = 100;

  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  Camera camera;

  HittableList world;

  auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto material_left = std::make_shared<Dielectric>(1.5);
  auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.3);

  world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0,
                                     material_ground));
  world.add(
      std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
  world.add(
      std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(
      std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.4, material_left));
  world.add(
      std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

  std::cout << "P3" << std::endl;
  std::cout << image_width << " " << image_height << std::endl;
  std::cout << 255 << std::endl;

  int progress = 0;
  int total_progress = image_height * image_width;
  for (int row = image_height - 1; row >= 0; --row) {
    for (int col = 0; col < image_width; ++col) {
      if (progress % 3600 == 0) {
        std::cerr << "Progress: " << (double)progress / total_progress * 100
                  << std::endl;
      }

      Color current_pixel_color = Color{0, 0, 0};

      // Perform a random sampling of pixels.
      for (int i = 0; i < samples_per_pixel; ++i) {
        auto col_fraction = (col + random_double(-1, 1)) / (image_width - 1);
        auto row_fraction = (row + random_double(-1, 1)) / (image_height - 1);

        const Ray ray = camera.get_ray(col_fraction, row_fraction);
        const Color pixel_color = ray_color(ray, world, 0);
        current_pixel_color = current_pixel_color + pixel_color;
      }

      write_color(std::cout, current_pixel_color, samples_per_pixel);
      ++progress;
    }
    std::cout << std::endl;
  }

  return 0;
}