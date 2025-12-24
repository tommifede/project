#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "namespace.hpp"
#include <vector>

namespace lotka_volterra {
struct State
{
  double x;
  double y;
  double H;
};
class Simulation
{
private:
  double dt_;
  double A_;
  double B_;
  double C_;
  double D_;
  double x_rel_;
  double y_rel_;
  std::vector<State> states_;

  void check_parameters(double dt, double A, double B, double C, double D,
                        double x0, double y0) const;
  void integrate();
  double compute_H(double x, double y) const;

public:
  Simulation(double dt, double A, double B, double C, double D, double x0 = 0.,
             double y0 = 0.);

  int steps() const;
  State const& state_at(int i) const;
  void evolve();
  void evolve_time(double T);
  void evolve_steps(int steps);
};
} // namespace lotka_volterra

#endif