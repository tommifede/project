#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <array>
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
  std::array<double, 4> pars_;
  std::vector<State> states_;
  double max_rel_drift_;
  bool unstable_ = false;

  void check_parameters(double dt, double A, double B, double C, double D, double x0, double y0) const;
  void integrate();
  double compute_H(double x, double y);

public:
  Simulation(double dt, double A, double B, double C, double D, double x0 = 0., double y0 = 0.);
  double dt() const;
  double H() const;
  double maxRelDrift() const;
  double const& getParameter(std::size_t i) const;
  std::size_t steps() const;
  State const& stateAt(std::size_t i) const;
  bool evolve();
  bool evolveSteps(std::size_t steps);
  bool evolveTime(double T);
  bool isUnstable() const;
};
} // namespace lotka_volterra

#endif