#pragma once
#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
 public:
  HittableList();
  HittableList(std::shared_ptr<Hittable> object);

  void clear();
  void add(std::shared_ptr<Hittable> object);

  virtual bool hit(const Ray& r,
                   double t_min,
                   double t_max,
                   HitRecord& rec) const override;

 private:
  std::vector<std::shared_ptr<Hittable>> objects;
};
