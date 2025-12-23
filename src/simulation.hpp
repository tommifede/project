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
  struct State_rel
  {
    double x_rel;
    double y_rel; 
  };
  Simulation(double dt, double A, double B, double C, double D, double x0 = 0.,
             double y0 = 0.);

  std::size_t steps() const;
  State const& state(std::size_t i) const;
  void evolve();

private:
  double A_;
  double B_;
  double C_;
  double D_;
  double dt_;
  double x_rel_;
  double y_rel_;
  std::vector<State> states_;
  std::vector<State_rel> states_rel_;

  void check_parameters(double dt, double A, double B, double C, double D,
                        double x0, double y0) const;
  void init_state();
  void integrate();
  double compute_H(double x, double y) const;
};
} // namespace lotka_volterra

#endif