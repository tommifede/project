#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/simulation.hpp"

using lotka_volterra::Simulation;

TEST_CASE("Simulation constructor works")
{
  Simulation sim(0.001, 1.0, 1.0, 1.0, 1.0, 10.0, 5.0);

  CHECK(sim.GetSteps() == 1);
}

TEST_CASE("Simulation evolves correctly")
{
  Simulation sim(0.001, 1.0, 1.0, 1.0, 1.0, 10.0, 5.0);

  sim.evolve();

  CHECK(sim.GetSteps() == 2);

  const auto& s = sim.GetState(1);
  CHECK(s.x >= 0.);
  CHECK(s.y >= 0.);
}

