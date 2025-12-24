#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "io.hpp"
#include "simulation.hpp"
#include <fstream>
#include <string>

namespace io {
void output_csv(lotka_volterra::Simulation const& simulation,
                std::string const& filename);
} // namespace io

#endif