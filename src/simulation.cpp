#include "../include/simulation.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

namespace lotka_volterra {
void Simulation::check_parameters(double dt, double A, double B, double C,
                                  double D, double x0, double y0) const
{
  if (dt <= 0.) {
    throw std::invalid_argument("dt must be > 0");
  }
  if (dt > 0.01 || dt < 0.0001) {
    throw std::invalid_argument("dt must be between 0.0001 and 0.01");
  }
  if (A <= 0. || B <= 0. || C <= 0. || D <= 0.) {
    throw std::invalid_argument("A, B, C, D must be > 0");
  }
  if (x0 < 0. || y0 < 0.) {
    throw std::invalid_argument("x0, y0 must be >= 0");
  }
}

void Simulation::integrate()
{
  const double x_prev = x_rel_;
  const double y_prev = y_rel_;
  const double x_new  = x_prev + A_ * (1 - y_prev) * x_prev * dt_;
  const double y_new  = y_prev + D_ * (x_prev - 1) * y_prev * dt_;
  x_rel_              = std::max(0., x_new);
  y_rel_              = std::max(0., y_new);
}

double Simulation::compute_H(double x, double y) const
{
  double H = (x > 0 && y > 0)
               ? (-D_ * std::log(x) + C_ * x + B_ * y - A_ * std::log(y))
               : std::numeric_limits<double>::infinity();
  return H;
}


Simulation::Simulation(double dt, double A, double B, double C, double D,
                       double x0, double y0)
    : dt_{dt}
    , A_{A}
    , B_{B}
    , C_{C}
    , D_{D}
    , x_rel_{x0 * C / D}
    , y_rel_{y0 * B / A}
{
  check_parameters(dt, A, B, C, D, x0, y0);
  states_.push_back({x0, y0, compute_H(x0, y0)});
}

double const& Simulation::GetPar(std::size_t i) const
{
  if (i > 3) {
    throw std::invalid_argument("NO");
  }
  return pars_[i];
}

std::size_t Simulation::steps() const
{
  return states_.size();
}

State const& Simulation::state_at(std::size_t i) const
{
  return states_.at(i);
}

void Simulation::evolve()
{
  State const& last_state = states_.back();
  x_rel_                  = last_state.x * C_ / D_;
  y_rel_                  = last_state.y * B_ / A_;
  integrate();
  double x_abs = x_rel_ * D_ / C_;
  double y_abs = y_rel_ * A_ / B_;
  states_.push_back({x_abs, y_abs, compute_H(x_abs, y_abs)});
}

void Simulation::evolve_time(double T)
{
  double n = T / dt_;
  if (std::abs(n - std::round(n)) > 1e-8) {
    throw std::invalid_argument("T must be multiple of dt");
  }
  std::size_t const tot_steps =
      steps() + static_cast<std::size_t>(std::round(n));
  for (std::size_t i = steps(); i < tot_steps; ++i) {
    evolve(); // MODIFICARE CON evolve_steps
  }
}

void Simulation::evolve_steps(std::size_t add_steps)
{
  /* std::size_t const tot_steps = steps() + add_steps;
  for (std::size_t i = steps(); i < tot_steps; ++i) {
    evolve();
  } */
  for (std::size_t i = 0; i < add_steps; ++i) {
    evolve();
  }
}
} // namespace lotka_volterra