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

double readDouble(std::string const& var, std::string const& prompt,
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


std::size_t readSize(std::string const& var, std::string const& prompt,
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

SimulationParameters askSimulationParameters()
{
  SimulationParameters sim_p;
  std::cout << "Insert simulation parameters values\n";

  sim_p.dt = readDouble("dt", "dt (double, 0.0001 <= dt <= 0.01): ", 0.0001,
                         0.01, false);
  sim_p.A  = readDouble("A", "A (double, >0): ", 0., 1e6);
  sim_p.B  = readDouble("B", "B (double, >0): ", 0., 1e6);
  sim_p.C  = readDouble("C", "C (double, >0): ", 0., 1e6);
  sim_p.D  = readDouble("D", "D (double, >0): ", 0., 1e6);
  sim_p.x0 =
      readDouble("prey population density",
                  "prey population density (double, >=0): ", 0., 1e6, false);
  sim_p.y0 = readDouble("predator population density",
                         "predator population density (double, >=0): ", 0., 1e6,
                         false);

  return sim_p;
}

std::size_t askRendererParameter()
{
  std::size_t size;
  std::cout << "Insert renderer parameter value\n";

  size = readSize("size", "size (integer, between 800 and 1000): ", 800, 1000);

  return size;
}

lotka_volterra::Simulation inputSimulation(SimulationParameters sim_p)
{
  lotka_volterra::Simulation simulation(sim_p.dt, sim_p.A, sim_p.B, sim_p.C,
                                        sim_p.D, sim_p.x0, sim_p.y0);

  return simulation;
}

lotka_volterra::Simulation inputSimulation()
{
  lotka_volterra::Simulation simulation =
      inputSimulation(askSimulationParameters());

  return simulation;
}

lotka_volterra::Renderer inputRenderer(std::size_t size)
{
  lotka_volterra::Renderer renderer(size);

  return renderer;
}

lotka_volterra::Renderer inputRenderer()
{
  lotka_volterra::Renderer renderer = inputRenderer(askRendererParameter());

  return renderer;
}

double inputTime(lotka_volterra::Simulation const& sim)
{
  std::cout << "Insert time of simulation, multiple of " << sim.dt() << "\n";

  double T = readDouble("T", "T (double, >0): ", 0, 1e8);
  double n = T / sim.dt();

  if (std::abs(n - std::round(n)) > 1e-8) {
    throw std::invalid_argument("T must be multiple of dt.");
  }

  return T;
}
} // namespace io