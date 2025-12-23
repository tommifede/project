#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "namespace.hpp"
#include <cstddef>
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

  Simulation(double dt, double A, double B, double C, double D, double x0 = 0.,
             double y0 = 0.);

  int GetSteps() const;
  State const& GetState(std::size_t i) const;
  void evolve();
  void evolve(double T);
  void evolve(int steps);

private:
  double dt_;
  double A_;
  double B_;
  double C_;
  double D_;
  double x_rel_;
  double y_rel_;
  std::vector<State> states_;
  int steps_;

  void check_parameters(double dt, double A, double B, double C, double D,
                        double x0, double y0) const;
  void integrate();
  double compute_H(double x, double y) const;
};
} // namespace lotka_volterra

#endif