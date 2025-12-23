#include "simulation.hpp"
#include <cmath>
#include <iostream>

namespace lotka_volterra {
Simulation::Simulation(double dt, double A, double B, double C, double D,
                       double x0, double y0)
    : dt_{dt}
    , A_{A}
    , B_{B}
    , C_{C}
    , D_{D}
    , x_rel_{x0}
    , y_rel_{y0}
{
  check_parameters(dt, A, B, C, D, x0, y0);
  init_state();
}

std::size_t Simulation::steps() const
{
  return states_.size();
}

Simulation::State const& Simulation::state(std::size_t i) const
{
  return states_.at(i);
}

void Simulation::evolve()
{
  State const& s = state(steps());
  integrate();
  states_rel_.push_back({x_rel_, y_rel_});
  double x_abs = x_rel_ * D_ / C_;
  double y_abs = y_rel_ * A_ / B_;
  double H     = (x_abs > 0 && y_abs > 0) ? compute_H(x_abs, y_abs)
                                          : std::numeric_limits<double>::infinity();
  states_.push_back({x_abs, y_abs, H}); 
}


void Simulation::check_parameters(double dt, double A, double B, double C,
                                  double D, double x0, double y0) const
{
  if (dt <= 0.) {
    std::cerr << "Error: dt must be > 0 !\n";
    std::exit(EXIT_FAILURE);
  }
  if (dt >= 0.01 || dt <= 0.0001) {
    std::cerr << "Error: dt must be between 0.0001 and 0.01 !\n";
    std::exit(EXIT_FAILURE);
  }
  if (A <= 0. || B <= 0. || C <= 0. || D <= 0.) {
    std::cerr << "Error: A, B, C, D must be > 0 !\n";
    std::exit(EXIT_FAILURE);
  }
  if (x0 < 0. || y0 < 0.) {
    std::cerr << "Error: x0, y0 must be >= 0 !\n";
    std::exit(EXIT_FAILURE);
  }
}

void Simulation::init_state()
{
  double H = (x_rel_ > 0 && y_rel_ > 0)
               ? compute_H(x_rel_, y_rel_)
               : std::numeric_limits<double>::infinity();
  states_.push_back({x_rel_, y_rel_, H});
  states_rel_.push_back({x_rel_, y_rel_});
}

void Simulation::integrate()
{
  x_rel_ = x_rel_ + A_ * (1 - y_rel_) * x_rel_ * dt_;
  y_rel_ = y_rel_ + D_ * (x_rel_ - 1) * y_rel_ * dt_;
}

double Simulation::compute_H(double x, double y) const
{
  /* if (x == 0. || y == 0.) {
    std::cerr << "Error: undefined H !";
    std::exit(EXIT_FAILURE);
  } */
  return -D_ * std::log(x) + C_ * x + B_ * y - A_ * std::log(y);
}
} // namespace lotka_volterra