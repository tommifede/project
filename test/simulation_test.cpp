#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "input.hpp"
#include "output.hpp"

TEST_CASE("Simulation constructor works and initialize correctly")
{
  lotka_volterra::Simulation sim(0.001, 1., 2., 3., 4., 5., 6.);

  CHECK(sim.steps() == 1);

  lotka_volterra::State const& s0 = sim.stateAt(0);
  CHECK(s0.x == 5.);
  CHECK(s0.y == 6.);

  double expected_H =
      -4. * std::log(5.) + 3. * 5. + 2. * 6. - 1. * std::log(6.);
  CHECK(s0.H == doctest::Approx(expected_H));
}

TEST_CASE("Simulation evolves correctly")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 10., 5.);

  sim.evolve();

  CHECK(sim.steps() == 2);

  lotka_volterra::State const& s = sim.stateAt(1);
  CHECK(s.x >= 0.);
  CHECK(s.y >= 0.);
}

TEST_CASE("lotka_volterra::Simulation constructor throws on invalid parameters")
{
  CHECK_THROWS(lotka_volterra::Simulation(0., 1., 1., 1., 1.));
  CHECK_THROWS(lotka_volterra::Simulation(0.00001, 1., 1., 1., 1.));
  CHECK_THROWS(lotka_volterra::Simulation(0.1, 1., 1., 1., 1.));
  CHECK_THROWS(lotka_volterra::Simulation(-0.1, 1., 1., 1., 1.));
  CHECK_THROWS(lotka_volterra::Simulation(0.001, -1., 1., 1., 1.));
  CHECK_THROWS(lotka_volterra::Simulation(0.001, 1., 0., 1., 1.));
  CHECK_THROWS(lotka_volterra::Simulation(0.001, 1., 1., 1., 1., -1., 0.));
  CHECK_THROWS(lotka_volterra::Simulation(0.001, 1., 1., 1., 1., 1., -1.));
}

TEST_CASE("Evolve with integer steps")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 10., 5.);
  sim.evolveSteps(100);

  CHECK(sim.steps() == 101);
}

TEST_CASE("Evolve with time T")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 10., 5.);
  sim.evolveTime(0.1);

  CHECK(sim.steps() == 101);
  CHECK_THROWS(sim.evolveTime(0.0001));
  CHECK_THROWS(sim.evolveTime(0.1115));
  CHECK_NOTHROW(sim.evolveTime(0.001));
}

TEST_CASE("Extinction of prey keeps x at zero")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 0., 5.);
  sim.evolveSteps(100);

  for (std::size_t i = 0; i < sim.steps(); ++i) {
    CHECK(sim.stateAt(i).x == 0.);
  }
}

TEST_CASE("Extinction of predator keeps y at zero")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 5., 0.);
  sim.evolveSteps(100);

  for (std::size_t i = 0; i < sim.steps(); ++i) {
    CHECK(sim.stateAt(i).y == 0.);
  }
}

TEST_CASE("H is infinite on extinction")
{
  lotka_volterra::Simulation sim1(0.001, 1., 1., 1., 1., 0., 5.);
  CHECK(std::isinf(sim1.stateAt(0).H));

  lotka_volterra::Simulation sim2(0.001, 1., 1., 1., 1., 5., 0.);
  CHECK(std::isinf(sim2.stateAt(0).H));
}

TEST_CASE("Convergence for dt -> 0")
{
  double A  = 1.;
  double B  = 1.;
  double C  = 1.;
  double D  = 1.;
  double x0 = 10.;
  double y0 = 5.;
  double T  = 1.;

  lotka_volterra::Simulation sim_dt1(0.01, A, B, C, D, x0, y0);
  sim_dt1.evolveTime(T);

  lotka_volterra::Simulation sim_dt2(0.001, A, B, C, D, x0, y0);
  sim_dt2.evolveTime(T);

  lotka_volterra::Simulation sim_dt3(0.0001, A, B, C, D, x0, y0);
  sim_dt3.evolveTime(T);

  lotka_volterra::State const& s1 = sim_dt1.stateAt(sim_dt1.steps() - 1);
  lotka_volterra::State const& s2 = sim_dt2.stateAt(sim_dt2.steps() - 1);
  lotka_volterra::State const& s3 = sim_dt3.stateAt(sim_dt3.steps() - 1);

  CHECK(s2.x == doctest::Approx(s3.x).epsilon(1e-3));
  CHECK(s2.y == doctest::Approx(s3.y).epsilon(1e-3));
  CHECK(s1.x == doctest::Approx(s2.x).epsilon(1e-2));
  CHECK(s1.y == doctest::Approx(s2.y).epsilon(1e-2));
}

TEST_CASE("H remains approximately constant")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 10., 5.);
  sim.evolveTime(1.);

  double H0 = sim.stateAt(0).H;

  for (std::size_t i = 1; i < sim.steps(); ++i) {
    lotka_volterra::State const& s = sim.stateAt(i);
    CHECK(s.H == doctest::Approx(H0).epsilon(1e-2));
  }
}

TEST_CASE("Trajectory convergence for dt -> 0")
{
  double A = 1., B = 1., C = 1., D = 1.;
  double x0 = 10., y0 = 5.;
  double T = 1.0;

  lotka_volterra::Simulation sim_dt1(0.01, A, B, C, D, x0, y0);
  sim_dt1.evolveTime(T);

  lotka_volterra::Simulation sim_dt2(0.001, A, B, C, D, x0, y0);
  sim_dt2.evolveTime(T);

  lotka_volterra::Simulation sim_dt3(0.0001, A, B, C, D, x0, y0);
  sim_dt3.evolveTime(T);

  std::size_t steps3 = sim_dt3.steps();
  for (std::size_t i3 = 0; i3 < steps3; ++i3) {
    double t       = static_cast<double>(i3) * 0.0001;
    std::size_t i1 = static_cast<std::size_t>(t / 0.01);
    std::size_t i2 = static_cast<std::size_t>(t / 0.001);

    lotka_volterra::State const& s1 =
        sim_dt1.stateAt(std::min(i1, sim_dt1.steps() - 1));
    lotka_volterra::State const& s2 =
        sim_dt2.stateAt(std::min(i2, sim_dt2.steps() - 1));
    lotka_volterra::State const& s3 = sim_dt3.stateAt(i3);

    CHECK(s2.x == doctest::Approx(s3.x).epsilon(1e-2));
    CHECK(s2.y == doctest::Approx(s3.y).epsilon(1e-2));
    CHECK(s1.x == doctest::Approx(s2.x).epsilon(1e-1));
    CHECK(s1.y == doctest::Approx(s2.y).epsilon(1e-1));

    double H0 = sim_dt3.stateAt(0).H;
    CHECK(s3.H == doctest::Approx(H0).epsilon(1e-3));
  }
}

TEST_CASE("Estimate numerical convergence order")
{
  double A = 1., B = 1., C = 1., D = 1.;
  double x0 = 10., y0 = 5.;
  double T = 1.0;

  double dt1 = 0.01;
  double dt2 = 0.001;
  double dt3 = 0.0001;

  lotka_volterra::Simulation sim1(dt1, A, B, C, D, x0, y0);
  sim1.evolveTime(T);
  lotka_volterra::Simulation sim2(dt2, A, B, C, D, x0, y0);
  sim2.evolveTime(T);
  lotka_volterra::Simulation sim3(dt3, A, B, C, D, x0, y0);
  sim3.evolveTime(T);

  lotka_volterra::State const& s1 = sim1.stateAt(sim1.steps() - 1);
  lotka_volterra::State const& s2 = sim2.stateAt(sim2.steps() - 1);
  lotka_volterra::State const& s3 = sim3.stateAt(sim3.steps() - 1);

  double error1 = std::abs(s1.x - s3.x);
  double error2 = std::abs(s2.x - s3.x);

  // Ordine di convergenza p ≈ log(error1/error2)/log(dt1/dt2)
  double p = std::log(error1 / error2) / std::log(dt1 / dt2);
  CHECK(p == doctest::Approx(1.).epsilon(0.01));
}

TEST_CASE("CSV output works correctly")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1.);
  sim.evolveTime(1.);
  io::outputCSV(sim, "trajectory.csv");
}

TEST_CASE("Simulation input creates a proper simulation")
{
  CHECK_NOTHROW(io::inputSimulation({0.001, 1., 1., 1., 1., 1., 1.}));

  lotka_volterra::Simulation sim =
      io::inputSimulation({0.001, 1., 1., 1., 1., 1., 1.});
  CHECK(sim.dt() == 0.001);

  CHECK_THROWS(io::inputSimulation({0.1, 1., 1., 1., 1., 1., 1.}));
  CHECK_THROWS(io::inputSimulation({0.01, 0., 1., 1., 1., 1., 1.}));
  CHECK_THROWS(io::inputSimulation({0.01, 1., -1., 1., 1., 1., 1.}));
  CHECK_THROWS(io::inputSimulation({0.1, 1., 1., 1., 1., -1., 1.}));
  CHECK_THROWS(io::inputSimulation({0.1, 1., 1., 1., 1., 1., -1.}));
}