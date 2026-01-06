#include "../include/input.hpp"
#include <charconv>
#include <cmath>
#include <iostream>

namespace io {
std::string trim(std::string& s)
{
  auto not_space = [](unsigned char c) { return !std::isspace(c); };

  s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
  s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());

  return s;
}

double read_double(std::string const& var, std::string const& prompt,
                   double min, double max, bool strict)
{
  while (true) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    trim(input);

    if (input.empty()) {
      throw std::invalid_argument(var + " input is empty.");
    }

    double val;
    auto [ptr, ec] =
        std::from_chars(input.data(), input.data() + input.size(), val);

    if (ec != std::errc{}) {
      throw std::invalid_argument("non-numeric input for " + var + ".");
    }

    size_t idx;
    double value = std::stod(input, &idx);

    if (idx != input.size()) {
      throw std::invalid_argument("non-numeric input for " + var + ".");
    }

    if ((strict && value <= min) || (!strict && value < min)) {
      throw std::out_of_range(var + " input out of range.");
    }

    if ((strict && value >= max) || (!strict && value > max)) {
      throw std::out_of_range(var + " input out of range.");
    }

    return value;
  }
}


std::size_t read_size(std::string const& var, std::string const& prompt,
                      std::size_t min, std::size_t max)
{
  while (true) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
      throw std::invalid_argument(var + " input is empty.");
    }

    std::size_t val;
    auto [ptr, ec] =
        std::from_chars(input.data(), input.data() + input.size(), val);

    if (ec != std::errc{}) {
      throw std::invalid_argument("non-numeric input for " + var + ".");
    }

    std::size_t idx;
    long long value = std::stoll(input, &idx);

    if (idx != input.size()) {
      throw std::invalid_argument(var + " input must be an integer.");
    }

    if (value < 0) {
      throw std::invalid_argument(var + " input must be positive.");
    }

    if (static_cast<std::size_t>(value) < min
        || static_cast<std::size_t>(value) > max) {
      throw std::out_of_range(var + " input out of range.");
    }

    return static_cast<std::size_t>(value);
  }
}

SimulationParams ask_simulation_params()
{
  SimulationParams sim_p;
  std::cout << "Insert simulation parameters values\n";

  sim_p.dt = read_double("dt", "dt (double, 0.0001 <= dt <= 0.01): ", 0.0001,
                         0.01, false);
  sim_p.A  = read_double("A", "A (double, >0): ", 0., 1e6);
  sim_p.B  = read_double("B", "B (double, >0): ", 0., 1e6);
  sim_p.C  = read_double("C", "C (double, >0): ", 0., 1e6);
  sim_p.D  = read_double("D", "D (double, >0): ", 0., 1e6);
  sim_p.x0 =
      read_double("prey population density",
                  "prey population density (double, >=0): ", 0., 1e6, false);
  sim_p.y0 = read_double("predator population density",
                         "predator population density (double, >=0): ", 0., 1e6,
                         false);

  return sim_p;
}

RendererParams ask_renderer_params()
{
  RendererParams ren_p;
  std::cout << "Insert renderer parameter value\n";

  ren_p.size =
      read_size("size", "size (integer, between 800 and 1000): ", 800, 1000);

  return ren_p;
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
  lotka_volterra::Renderer renderer(ren_p.size);

  return renderer;
}

lotka_volterra::Renderer input_renderer()
{
  lotka_volterra::Renderer renderer = input_renderer(ask_renderer_params());

  return renderer;
}

double input_T(lotka_volterra::Simulation const& sim)
{
  std::cout << "Insert time of simulation, multiple of " << sim.dt() << "\n";

  double T = read_double("T", "T (double, >0): ", 0, 1e8);
  double n = T / sim.dt();

  if (std::abs(n - std::round(n)) > 1e-8) {
    throw std::invalid_argument("T must be multiple of dt.");
  }

  return T;
}

} // namespace io