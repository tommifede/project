#ifndef SIMULATION_HPP
#define SIMULATION_HPP

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
  double x_rel_;
  double y_rel_;
  std::vector<double> pars_;
  std::vector<State> states_;

  void check_parameters(double dt, double A, double B, double C, double D, double x0, double y0) const;
  void integrate();
  double compute_H(double x, double y) const;

public:
  // Simulation();
  Simulation(double dt, double A, double B, double C, double D, double x0 = 0., double y0 = 0.);
  double dt() const;
  double const& getParameter(std::size_t i) const;
  std::size_t steps() const;
  State const& stateAt(std::size_t i) const;
  void evolve();
  void evolveSteps(std::size_t steps);
  void evolveTime(double T);
};
} // namespace lotka_volterra

#endif