#ifndef INPUT_HPP
#define INPUT_HPP

#include "io.hpp"
#include "renderer.hpp"
#include "simulation.hpp"
#include <cstddef>

namespace io {
struct SimulationParams
{
  double dt;
  double A;
  double B;
  double C;
  double D;
  double x0;
  double y0;
};

struct RendererParams
{
  std::size_t width;
  std::size_t height;
};

double read_double(std::string const& var, std::string const& prompt,
                   double min, double max, bool strict_min = true);
std::size_t read_size(std::string const& var, std::string const& prompt,
                      std::size_t min, std::size_t max);
SimulationParams ask_simulation_params();
RendererParams ask_renderer_params();
lotka_volterra::Simulation input_simulation(SimulationParams sim_p);
lotka_volterra::Simulation input_simulation();
lotka_volterra::Renderer input_renderer(RendererParams ren_p);
lotka_volterra::Renderer input_renderer();
double input_T(lotka_volterra::Simulation const& sim);

} // namespace io

#endif