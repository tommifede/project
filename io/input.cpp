#include "../include/input.hpp"
#include <cstddef>
#include <iostream>

namespace io {
double read_double(std::string const& prompt, double min, double max,
                   bool strict_min = true)
{
  while (true) {
    try {
      std::cout << prompt;
      std::string input;
      std::getline(std::cin, input);

      size_t idx;
      double value = std::stod(input, &idx);

      if (idx != input.size())
        throw std::invalid_argument("Non-numeric input");

      if ((strict_min && value <= min) || (!strict_min && value < min))
        throw std::out_of_range("Input out of range");

      if (value > max)
        throw std::out_of_range("Input out of range");

      return value;
    } catch (const std::invalid_argument&) {
      std::cerr << "Fatal error: non-numeric input.\n";
    } catch (const std::out_of_range&) {
      std::cerr << "Fatal error: input must be in [" << min << ", " << max
                << "].\n";
    }
  }
}

std::size_t read_size(const std::string& prompt, std::size_t min,
                      std::size_t max)
{
  while (true) {
    try {
      std::cout << prompt;
      std::string input;
      std::getline(std::cin, input);

      size_t idx;
      long long value = std::stoll(input, &idx);

      if (idx != input.size() || value < 0)
        throw std::invalid_argument("Non-integer input");

      if (static_cast<std::size_t>(value) < min
          || static_cast<std::size_t>(value) > max)
        throw std::out_of_range("Input out of range");

      return static_cast<std::size_t>(value);
    } catch (...) {
      std::cerr << "Fatal error: input must be an integer in [" << min << ", "
                << max << "].\n";
    }
  }
}

SimulationParams ask_simulation_params()
{
  SimulationParams p;

  std::cout
      << "Insert simulation parameters values \ndt (0.0001 < dt <= 0.01): ";
  std::string dt;
  std::cin >> dt;
  p.dt = read_double(dt, 0.0001, 0.01);

  std::cout << "A (>0): ";
  std::string A;
  std::cin >> A;
  p.A = read_double(A, 0., 1e6);

  std::cout << "B (>0): ";
  std::string B;
  std::cin >> B;
  p.B = read_double(B, 0., 1e6);

  std::cout << "C (>0): ";
  std::string C;
  std::cin >> C;
  p.C = read_double(C, 0., 1e6);

  std::cout << "D (>0): ";
  std::string D;
  std::cin >> D;
  p.D = read_double(D, 0., 1e6);

  std::cout << "x0 (>=0): ";
  std::string x0;
  std::cin >> x0;
  p.x0 = read_double(x0, 0., 1e6, false);

  std::cout << "y0 (>=0): ";
  std::string y0;
  std::cin >> y0;
  p.y0 = read_double(y0, 0., 1e6, false);

  return p;
}

RendererParams ask_renderer_params()
{
  RendererParams p;

  std::cout << "Insert renderer parameters values \nwidth and height (in "
               "pixel, 600 <= w <=1000): ";
  std::string dim;
  std::cin >> dim;
  p.width  = read_size(dim, 600, 1000);
  p.height = p.width;

  return p;
}

lotka_volterra::Simulation input_simulation(SimulationParams sim_p)
{
  lotka_volterra::Simulation simulation(sim_p.dt, sim_p.A, sim_p.B, sim_p.C,
                                        sim_p.D, sim_p.x0, sim_p.y0);
  return simulation;
}

lotka_volterra::Simulation input_simulation()
{
  lotka_volterra::Simulation simulation =
      input_simulation(ask_simulation_params());
  return simulation;
}

lotka_volterra::Renderer input_renderer(RendererParams ren_p)
{
  lotka_volterra::Renderer renderer(ren_p.width, ren_p.height);
  return renderer;
}

lotka_volterra::Renderer input_renderer()
{
  lotka_volterra::Renderer renderer = input_renderer(ask_renderer_params());
  return renderer;
}

} // namespace io