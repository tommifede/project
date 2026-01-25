#include "../include/simulation.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>

namespace lotka_volterra {
void Simulation::check_parameters(double dt, double A, double B, double C,
                                  double D, double x0, double y0) const
{
  if (dt <= 0.) {
    throw std::invalid_argument("dt must be > 0.");
  }
  if (dt != std::clamp(dt, 0.0001, 0.01)) {
    throw std::invalid_argument("dt must be between 0.0001 and 0.01.");
  }
  if (A <= 0. || B <= 0. || C <= 0. || D <= 0.) {
    throw std::invalid_argument("A, B, C, D must be > 0.");
  }
  if (x0 < 0. || y0 < 0.) {
    throw std::invalid_argument("x0, y0 must be >= 0.");
  }
}

void Simulation::integrate()
{
  double A = getParameter(0);
  double D = getParameter(3);
  const double x_prev = x_rel_;
  const double y_prev = y_rel_;
  const double x_new  = x_prev + A * (1 - y_prev) * x_prev * dt_;
  const double y_new  = y_prev + D * (x_prev - 1) * y_prev * dt_;
  x_rel_              = std::max(0., x_new);
  y_rel_              = std::max(0., y_new);
}

double Simulation::compute_H(double x, double y) const
{
  double A = getParameter(0);
  double B = getParameter(1);
  double C = getParameter(2);
  double D = getParameter(3);
  double H = (x > 0 && y > 0)
               ? (-D * std::log(x) + C * x + B * y - A * std::log(y))
               : std::numeric_limits<double>::infinity();
  return H;
}


Simulation::Simulation(double dt, double A, double B, double C, double D,
                       double x0, double y0)
    : dt_{dt}
    , x_rel_{x0 * C / D}
    , y_rel_{y0 * B / A}
    , pars_{{A, B, C, D}}
{
  check_parameters(dt, A, B, C, D, x0, y0);
  states_.push_back({x0, y0, compute_H(x0, y0)});
}

double Simulation::dt() const
{
  return dt_;
}

double const& Simulation::getParameter(std::size_t i) const
{
  if (i > 3) {
    throw std::invalid_argument("parameter index out of range.");
  }
  return pars_[i];
}

std::size_t Simulation::steps() const
{
  return states_.size();
}

State const& Simulation::stateAt(std::size_t i) const
{
  return states_.at(i);
}

void Simulation::evolve()
{
  State const& last_state = states_.back();
  double A                = getParameter(0);
  double B                = getParameter(1);
  double C                = getParameter(2);
  double D                = getParameter(3);
  x_rel_                  = last_state.x * C / D;
  y_rel_                  = last_state.y * B / A;
  integrate();
  double x_abs = x_rel_ * D / C;
  double y_abs = y_rel_ * A / B;
  states_.push_back({x_abs, y_abs, compute_H(x_abs, y_abs)});
}

void Simulation::evolveSteps(std::size_t add_steps)
{
  for (std::size_t i = 0; i < add_steps; ++i) {
    evolve();
  }
}

void Simulation::evolveTime(double T)
{
  if (T < 0) {
    throw std::invalid_argument("T must be positive.");
  }

  double n = T / dt_;
  if (std::abs(n - std::round(n)) > 1e-8) {
    throw std::invalid_argument("T must be multiple of dt.");
  }

  std::size_t steps = static_cast<std::size_t>(std::round(n));
  evolveSteps(steps);
}
} // namespace lotka_volterra