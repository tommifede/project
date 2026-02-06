#ifndef INPUT_HPP
#define INPUT_HPP

#include "renderer.hpp"

namespace io {
struct SimulationParameters
{
  double dt;
  double A;
  double B;
  double C;
  double D;
  double x0;
  double y0;
};

std::string trim(std::string& s);
double readDouble(std::string const& var, std::string const& prompt, double min, double max, bool strict = true);
std::size_t readSize(std::string const& var, std::string const& prompt, std::size_t min, std::size_t max);
SimulationParameters askSimulationParameters();
std::size_t askSize();
lotka_volterra::Simulation inputSimulation(SimulationParameters sim_p);
lotka_volterra::Simulation inputSimulation();
lotka_volterra::Renderer inputRenderer(std::size_t size);
lotka_volterra::Renderer inputRenderer();
double inputTime(lotka_volterra::Simulation const& sim);
} // namespace io

#endif