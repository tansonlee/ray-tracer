cmake_minimum_required(VERSION 3.10)

project(RayTracer
    DESCRIPTION "Ray tracer!"
    LANGUAGES CXX)

add_executable(main
    src/camera.cpp
    src/camera.h
    src/color.cpp
    src/color.h
    src/helpers.cpp
    src/helpers.h
    src/ray.cpp
    src/ray.h
    src/vec3.cpp
    src/vec3.h
    src/hittables/hittable_list.cpp
    src/hittables/hittable_list.h
    src/hittables/sphere.cpp
    src/hittables/sphere.h
    src/hittables/hittable.h
    src/materials/dielectric.cpp
    src/materials/dielectric.h
    src/materials/lambertian.cpp
    src/materials/lambertian.h
    src/materials/metal.cpp
    src/materials/metal.h
    src/materials/material.h
    src/main.cpp)

target_compile_features(main PRIVATE cxx_std_11)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -pthread)
