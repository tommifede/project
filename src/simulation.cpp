#include "simulation.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>

namespace lotka_volterra {
void Simulation::check_parameters(double dt, double A, double B, double C, double D, double x0, double y0) const
{
  if (dt != std::clamp(dt, 0.0001, 0.01)) {
    throw std::invalid_argument("parameter dt must be between 0.0001 and 0.01.");
  }
  if (A <= 0. || B <= 0. || C <= 0. || D <= 0.) {
    throw std::invalid_argument("parameters A, B, C, D must be > 0.");
  }
  if (x0 < 0. || y0 < 0.) {
    throw std::invalid_argument("parameters x0, y0 must be >= 0.");
  }
}

void Simulation::integrate()
{
  double const A = pars_[0];
  double const D = pars_[3];

  double const x = x_rel_; // x_(i)
  double const y = y_rel_; // y_(i)

  double const x_next = x + A * (1 - y) * x * dt_; // x_(i+1)
  double const y_next = y + D * (x - 1) * y * dt_; // y_(i+1)

  x_rel_ = std::max(0., x_next);
  y_rel_ = std::max(0., y_next);
}

double Simulation::compute_H(double x, double y)
{
  double const A = pars_[0];
  double const B = pars_[1];
  double const C = pars_[2];
  double const D = pars_[3];

  double H = (x > 0 && y > 0) ? (-D * std::log(x) + C * x + B * y - A * std::log(y)) : std::numeric_limits<double>::infinity();
  if (!std::isfinite(H)) {
    unstable_ = true;
  }
  return H;
}

Simulation::Simulation(double dt, double A, double B, double C, double D, double x0, double y0)
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

double Simulation::H() const
{
  return states_.back().H;
}

double Simulation::maxRelDrift() const
{
  return max_rel_drift_;
}

double const& Simulation::getParameter(std::size_t i) const
{
  if (i > 3) {
    throw std::out_of_range("parameter index out of range.");
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

bool Simulation::evolve()
{
  double const A = pars_[0];
  double const B = pars_[1];
  double const C = pars_[2];
  double const D = pars_[3];

  State const& curr_state = states_.back();
  x_rel_                  = curr_state.x * C / D; // from x to x_rel
  y_rel_                  = curr_state.y * B / A; // from y to y_rel

  integrate();

  double x_next = x_rel_ * D / C; // from x_rel to x
  double y_next = y_rel_ * A / B; // from y_rel to y

  double H_next = compute_H(x_next, y_next);

  if (states_.size() >= 2) {
    double const H_tol = 50. * dt_;                                                // energy relative tolerance (dH/H = O(dt))
    double rel_drift   = std::abs(H_next - curr_state.H) / std::abs(curr_state.H); // energy relative variation

    if (rel_drift > H_tol) {
      unstable_      = true;
      max_rel_drift_ = rel_drift;
      return false;
    }
  }
  states_.push_back({x_next, y_next, H_next});
  return true;
}

bool Simulation::evolveSteps(std::size_t add_steps)
{ // "overload" of evolve
  for (std::size_t i = 0; i < add_steps; ++i) {
    if (!evolve()) {
      return false;
    }
  }
  return true;
}

bool Simulation::evolveTime(double T)
{ // "overload" of evolveSteps
  if (T < 0) {
    throw std::invalid_argument("parameter T must be positive.");
  }

  double n = T / dt_;
  if (std::abs(n - std::round(n)) > 1e-8) {
    throw std::invalid_argument("parameter T must be multiple of dt.");
  }

  std::size_t steps = static_cast<std::size_t>(std::round(n));
  return evolveSteps(steps);
}

bool Simulation::isUnstable() const
{
  return unstable_;
}
} // namespace lotka_volterra