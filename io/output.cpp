#include "../include/output.hpp"
#include <fstream>

namespace io {
void output_csv(lotka_volterra::Simulation const& simulation,
                std::string const& filename)
{
  std::ofstream file(filename);
  if (!file) {
    throw std::runtime_error("Can't open file");
  }

  file << "step,x,y,H\n";
  for (std::size_t i = 0; i < simulation.steps(); ++i) {
    lotka_volterra::State const& state = simulation.state_at(i);
    file << i << "," << state.x << "," << state.y << "," << state.H << "\n";
  }
}
} // namespace io