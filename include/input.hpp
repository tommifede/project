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

double read_double(std::string const& prompt, double min, double max,
                   bool strict_min = true);
SimulationParams ask_simulation_params();
RendererParams ask_renderer_params();
lotka_volterra::Simulation input_simulation(SimulationParams sim_p);
lotka_volterra::Simulation input_simulation();
lotka_volterra::Renderer input_renderer(RendererParams ren_p);
lotka_volterra::Renderer input_renderer();

} // namespace io

#endif