#include "output.hpp"
#include <fstream>
#include <iostream>

namespace io {
void outputStatus(lotka_volterra::Simulation const& simulation)
{
  std::cout << "\nSimulation finished\n";
  std::cout << "Total steps:" << simulation.steps() << "\n";
  std::cout << "Status: ";
  if (simulation.isUnstable()) {
    std::cout << "ABORTED (unstable) | H = " << simulation.H() << " | dH/H = " << simulation.maxRelDrift() << "\n";
  } else {
    std::cout << "COMPLETED\n";
  }
}
void outputCSV(lotka_volterra::Simulation const& simulation, std::string const& filename)
{
  std::ofstream file(filename);

  if (!file) {
    throw std::runtime_error("cannot open file.");
  }

  file << "t,x,y,H\n";
  for (std::size_t i = 0; i < simulation.steps(); ++i) {
    lotka_volterra::State const& state = simulation.stateAt(i);
    file << static_cast<int>(i) * simulation.dt() << "," << state.x << "," << state.y << "," << state.H << "\n";
  }
}
} // namespace io