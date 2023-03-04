#include "color.h"
#include <cmath>
#include "helpers.h"

std::ostream& write_color(std::ostream& out, Color color, int num_samples) {
  const double multiplier = 1.0 / num_samples;
  // sqrt for gamma correcting where gamma = 2.
  const double r = sqrt(color.get_x() * multiplier);
  const double g = sqrt(color.get_y() * multiplier);
  const double b = sqrt(color.get_z() * multiplier);

  return out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << " "
             << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << " "
             << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << std::endl;
}