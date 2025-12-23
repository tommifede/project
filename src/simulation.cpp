#include "simulation.hpp"
#include <iostream>

namespace lotka_volterra {
Simulation::Simulation(double x0 = 0., double y0 = 0., double dt)
    : x_rel_{x0}
    , y_rel_{y0}
    , dt_{dt}
{
  if (x0 < 0. || y0 < 0. || dt < 0.) {
    std::cerr << "Error: x0, y0 and dt must be >= 0!\n";
    std::exit(EXIT_FAILURE);
  }
  states_.push_back({x_rel_, y_rel_, compute_H(x_rel_, y_rel_)});
}
} // namespace lotka_volterra