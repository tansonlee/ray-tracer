#include "hittable_list.h"

HittableList::HittableList() {}

HittableList::HittableList(std::shared_ptr<Hittable> object) {
  this->add(object);
}

void HittableList::clear() {
  this->objects.clear();
}

void HittableList::add(std::shared_ptr<Hittable> object) {
  this->objects.push_back(object);
}

bool HittableList::hit(const Ray& r,
                       double t_min,
                       double t_max,
                       HitRecord& rec) const {
  HitRecord temp_rec;
  bool hit_anything = false;
  auto closest_so_far = t_max;

  for (const auto& object : objects) {
    if (object->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}