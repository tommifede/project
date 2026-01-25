#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "simulation.hpp"
#include <string>

namespace io {
void outputCSV(lotka_volterra::Simulation const& simulation,
                std::string const& filename);
} // namespace io

#endif 