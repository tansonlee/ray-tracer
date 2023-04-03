#include <cmath>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
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

int max_depth;
int samples_per_pixel;
double aspect_ratio;
int image_width;
int image_height;

int total_progress;
int progress = 0;

Color* result;

Color get_result_pixel(int row, int col) {
  return result[row * image_width + col];
}

void set_result_pixel(int row, int col, Color val) {
  result[row * image_width + col] = val;
}

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
  if (depth > max_depth) {
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
  set_result_pixel(row, col, current_pixel_color);
  // std::cerr << "finished pixel " << row << " " << col << std::endl;
}

// Computes pixels in range [start, end)
void compute_range_of_pixels(int start,
                             int end,
                             const Camera& camera,
                             const HittableList& world) {
  for (int i = start; i < end; ++i) {
    int pixel_row = i / image_width;
    int pixel_col = i % image_width;
    compute_color_for_pixel(pixel_row, pixel_col, camera, world);
  }
}

HittableList scene_1() {
  HittableList world;

  auto ground_material =
      std::make_shared<Lambertian>(Color(0.3373, 0.4902, 0.2745));
  world.add(
      std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  auto material2 = std::make_shared<Metal>(
      Color(0.6588235294, 0.662745098, 0.6784313725), 0.5);
  world.add(std::make_shared<Sphere>(Point3(0, 0.9, 1), 0.9, material2));

  auto material3 =
      std::make_shared<Lambertian>(Color(1, 0.3254901961, 0.462745098));
  world.add(std::make_shared<Sphere>(Point3(1.7, 0.7, 2), 0.7, material3));

  auto material4 = std::make_shared<Metal>(Color(1, 0, 0), 0);
  world.add(std::make_shared<Sphere>(Point3(0, 0.3, 3), 0.3, material4));

  auto material5 = std::make_shared<Dielectric>(1.5);
  world.add(std::make_shared<Sphere>(Point3(-1, 0.4, 4.8), 0.4, material5));

  auto material6 = std::make_shared<Lambertian>(Color(0, 0, 1));
  world.add(std::make_shared<Sphere>(Point3(-1.2, 0.13, 3.8), 0.13, material6));

  auto material7 = std::make_shared<Lambertian>(Color(0, 1, 1));
  world.add(std::make_shared<Sphere>(Point3(-1.5, 0.1, 3.5), 0.1, material7));

  auto material8 = std::make_shared<Metal>(
      Color(0.6588235294, 0.662745098, 0.6784313725), 0.0);
  world.add(std::make_shared<Sphere>(Point3(-2.2, 1.5, -1), 1.5, material8));

  for (double x = -8.5; x < -1; x += 0.6) {
    for (double z = -1.5; z < 5.5; z += 0.6) {
      double x_offset = random_double(-0.3, 0.3);
      double z_offset = random_double(-0.3, 0.3);
      double x_pos = x + x_offset;
      double z_pos = z + z_offset;
      if ((x_pos + 2.2) * (x_pos + 2.2) + (z_pos + 1) * (z_pos + 1) < 0.6) {
        continue;
      }

      double size = random_double(0.08, 0.14);
      double material_choice = random_double();
      if (material_choice < 0.5) {
        auto material =
            std::make_shared<Lambertian>(random_vec3() * random_vec3());
        world.add(std::make_shared<Sphere>(Point3(x_pos, size, z_pos), size,
                                           material));
      } else if (material_choice < 0.8) {
        double fuzz = random_double();
        auto material =
            std::make_shared<Metal>(random_vec3() * random_vec3(), fuzz);
        world.add(std::make_shared<Sphere>(Point3(x_pos, size, z_pos), size,
                                           material));

      } else {
        auto material = std::make_shared<Dielectric>(random_double());
        world.add(std::make_shared<Sphere>(Point3(x_pos, size, z_pos), size,
                                           material));
      }
    }
  }

  for (double x = -0.3; x < 3.0; x += 0.5) {
    for (double z = 4.0; z < 7.0; z += 0.5) {
      double x_offset = random_double(-0.2, 0.2);
      double z_offset = random_double(-0.2, 0.2);

      double size = random_double(0.08, 0.18);
      double material_choice = random_double();
      if (material_choice < 0.5) {
        auto material =
            std::make_shared<Lambertian>(random_vec3() * random_vec3());
        world.add(std::make_shared<Sphere>(
            Point3(x + x_offset, size, z + z_offset), size, material));
      } else if (material_choice < 0.8) {
        double fuzz = random_double();
        auto material =
            std::make_shared<Metal>(random_vec3() * random_vec3(), fuzz);
        world.add(std::make_shared<Sphere>(
            Point3(x + x_offset, size, z + z_offset), size, material));

      } else {
        auto material = std::make_shared<Dielectric>(random_double());
        world.add(std::make_shared<Sphere>(
            Point3(x + x_offset, size, z + z_offset), size, material));
      }
    }
  }

  return world;
}

int main() {
  max_depth = 30;
  samples_per_pixel = 500;
  aspect_ratio = 16.0 / 9.0;
  image_width = 1200;
  image_height = static_cast<int>(image_width / aspect_ratio);

  total_progress = image_height * image_width;

  const int num_threads = 8;
  const int pixels_per_thread = ceil(total_progress / num_threads);

  result = new Color[image_height * image_width];

  Point3 lookfrom(0, 1, 9);
  Point3 lookat(-0.8, 0, -8);
  Vec3 vup(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.0;

  Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
                dist_to_focus);

  HittableList world = scene_1();

  std::cout << "P3" << std::endl;
  std::cout << image_width << " " << image_height << std::endl;
  std::cout << 255 << std::endl;

  std::thread threads[num_threads];

  for (int i = 0; i < num_threads; ++i) {
    std::cerr << "Thread #" << i << " is computing range from "
              << std::min(i * pixels_per_thread, total_progress) << " to "
              << std::min((i + 1) * pixels_per_thread, total_progress)
              << std::endl;

    threads[i] = std::thread{
        compute_range_of_pixels,
        std::min(i * pixels_per_thread, total_progress),
        std::min((i + 1) * pixels_per_thread, total_progress), camera, world};
  }

  for (int i = 0; i < num_threads; ++i) {
    threads[i].join();
  }

  std::cerr << "Finished computing" << std::endl;

  for (int row = image_height - 1; row >= 0; --row) {
    for (int col = 0; col < image_width; ++col) {
      write_color(std::cout, get_result_pixel(row, col), samples_per_pixel);
    }
    std::cout << std::endl;
  }

  return 0;
}