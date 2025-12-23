#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "namespace.hpp"
#include <vector>

namespace lotka_volterra {
class Simulation
{
public:
  struct State
  {
    double x;
    double y;
    double H;
  };

  Simulation(double x0, double y0, double dt);
  Simulation(double A, double B, double C, double D, double x0, double y0,
             double dt);


  void evolve();
  std::size_t steps() const;
  State const& state(std::size_t i) const;

private:
  double A_{0.};
  double B_{0.};
  double C_{0.};
  double D_{0.};
  double dt_;
  double x_rel_;
  double y_rel_;
  std::vector<State> states_;
  double compute_H(double x, double y) const;
  void integrate();
};
} // namespace lotka_volterra

#endif