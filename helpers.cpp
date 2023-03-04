#include "helpers.h"

// Utility functions.
double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

double random_double() {
  // rand() returns an integer in the range [0, RAND_MAX] so this returns a
  // double in the range [0, 1).
  return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

// Clamp n to the interval [min, max].
double clamp(double n, double min, double max) {
  return n >= max ? max : n <= min ? min : n;
}
