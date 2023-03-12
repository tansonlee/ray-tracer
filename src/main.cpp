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
const int samples_per_pixel = 100;
const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 600;
const int image_height = static_cast<int>(image_width / aspect_ratio);

const int total_progress = image_height * image_width;
int progress = 0;

Color result[image_height][image_width];

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

HittableList random_scene() {
  HittableList world;

  auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add(
      std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  const int sqrt_num_spheres = 5;
  for (int a = 0; a < sqrt_num_spheres; a++) {
    for (int b = 0; b < sqrt_num_spheres; b++) {
      // Turn `a` to a number from -11 to 10.
      // Turn `b` to a number from -11 to 10.
      int x = map(a, 0, sqrt_num_spheres, -6, 6);
      int y = map(b, 0, sqrt_num_spheres, -6, 6);

      auto choose_mat = random_double();
      Point3 center(x + 0.9 * random_double(), 0.2, y + 0.9 * random_double());

      if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<Material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = random_vec3() * random_vec3();
          sphere_material = std::make_shared<Lambertian>(albedo);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = random_vec3(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = std::make_shared<Metal>(albedo, fuzz);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = std::make_shared<Dielectric>(1.5);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  return world;
}

void compute_color_for_pixel(int row,
                             int col,
                             const Camera& camera,
                             const HittableList& world) {
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

  ++progress;
  result[row][col] = current_pixel_color;
}

int main() {
  Point3 lookfrom(13, 2, 3);
  Point3 lookat(0, 0, 0);
  Vec3 vup(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;

  Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
                dist_to_focus);

  HittableList world = random_scene();

  std::cout << "P3" << std::endl;
  std::cout << image_width << " " << image_height << std::endl;
  std::cout << 255 << std::endl;

  for (int row = image_height - 1; row >= 0; --row) {
    for (int col = 0; col < image_width; ++col) {
      compute_color_for_pixel(row, col, camera, world);
    }
    std::cout << std::endl;
  }

  for (int row = image_height - 1; row >= 0; --row) {
    for (int col = 0; col < image_width; ++col) {
      write_color(std::cout, result[row][col], samples_per_pixel);
    }
    std::cout << std::endl;
  }

  return 0;
}