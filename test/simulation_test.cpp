#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/simulation.hpp"
#include <cmath>

TEST_CASE("Simulation constructor works and initialize correctly")
{
  lotka_volterra::Simulation sim(0.001, 1., 2., 3., 4., 5., 6.);

  CHECK(sim.steps() == 1);

  lotka_volterra::State const& s0 = sim.state_at(0);
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

  lotka_volterra::State const& s = sim.state_at(1);
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
  sim.evolve_steps(100);

  CHECK(sim.steps() == 101);
}

TEST_CASE("Evolve with time T")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 10., 5.);
  sim.evolve_time(0.1);

  CHECK(sim.steps() == 101);
}

TEST_CASE("Extinction of prey keeps x at zero")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 0., 5.);
  sim.evolve_steps(100);

  for (int i = 0; i < sim.steps(); ++i) {
    CHECK(sim.state_at(i).x == 0.);
  }
}

TEST_CASE("Extinction of predator keeps y at zero")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 5., 0.);
  sim.evolve_steps(100);

  for (int i = 0; i < sim.steps(); ++i) {
    CHECK(sim.state_at(i).y == 0.);
  }
}

TEST_CASE("H is infinite on extinction")
{
  lotka_volterra::Simulation sim1(0.001, 1., 1., 1., 1., 0., 5.);
  CHECK(std::isinf(sim1.state_at(0).H));

  lotka_volterra::Simulation sim2(0.001, 1., 1., 1., 1., 5., 0.);
  CHECK(std::isinf(sim2.state_at(0).H));
}

TEST_CASE("Convergence for dt -> 0")
{
  double A = 1., B = 1., C = 1., D = 1.;
  double x0 = 10., y0 = 5.;
  double T = 1.;

  lotka_volterra::Simulation sim_dt1(0.01, A, B, C, D, x0, y0);
  sim_dt1.evolve_time(T);

  lotka_volterra::Simulation sim_dt2(0.001, A, B, C, D, x0, y0);
  sim_dt2.evolve_time(T);

  lotka_volterra::Simulation sim_dt3(0.0001, A, B, C, D, x0, y0);
  sim_dt3.evolve_time(T);

  lotka_volterra::State const& s1 = sim_dt1.state_at(sim_dt1.steps() - 1);
  lotka_volterra::State const& s2 = sim_dt2.state_at(sim_dt2.steps() - 1);
  lotka_volterra::State const& s3 = sim_dt3.state_at(sim_dt3.steps() - 1);

  CHECK(s2.x == doctest::Approx(s3.x).epsilon(1e-3));
  CHECK(s2.y == doctest::Approx(s3.y).epsilon(1e-3));
  CHECK(s1.x == doctest::Approx(s2.x).epsilon(1e-2));
  CHECK(s1.y == doctest::Approx(s2.y).epsilon(1e-2));
}

TEST_CASE("H remains approximately constant")
{
  lotka_volterra::Simulation sim(0.001, 1., 1., 1., 1., 10., 5.);
  sim.evolve_time(1.);

  double H0 = sim.state_at(0).H;

  for (int i = 1; i < sim.steps(); ++i) {
    lotka_volterra::State const& s = sim.state_at(i);
    CHECK(s.H == doctest::Approx(H0).epsilon(1e-2));
  }
}

TEST_CASE("Trajectory convergence for dt -> 0")
{
  double A = 1., B = 1., C = 1., D = 1.;
  double x0 = 10., y0 = 5.;
  double T = 1.0;

  lotka_volterra::Simulation sim_dt1(0.01, A, B, C, D, x0, y0);
  sim_dt1.evolve_time(T);

  lotka_volterra::Simulation sim_dt2(0.001, A, B, C, D, x0, y0);
  sim_dt2.evolve_time(T);

  lotka_volterra::Simulation sim_dt3(0.0001, A, B, C, D, x0, y0);
  sim_dt3.evolve_time(T);

  int steps3 = sim_dt3.steps();
  for (int i3 = 0; i3 < steps3; ++i3) {
    double t = i3 * 0.0001;
    int i1   = static_cast<int>(t / 0.01);
    int i2   = static_cast<int>(t / 0.001);

    lotka_volterra::State const& s1 =
        sim_dt1.state_at(std::min(i1, sim_dt1.steps() - 1));
    lotka_volterra::State const& s2 =
        sim_dt2.state_at(std::min(i2, sim_dt2.steps() - 1));
    lotka_volterra::State const& s3 = sim_dt3.state_at(i3);

    CHECK(s2.x == doctest::Approx(s3.x).epsilon(1e-2));
    CHECK(s2.y == doctest::Approx(s3.y).epsilon(1e-2));
    CHECK(s1.x == doctest::Approx(s2.x).epsilon(1e-1));
    CHECK(s1.y == doctest::Approx(s2.y).epsilon(1e-1));

    double H0 = sim_dt3.state_at(0).H;
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
  sim1.evolve_time(T);
  lotka_volterra::Simulation sim2(dt2, A, B, C, D, x0, y0);
  sim2.evolve_time(T);
  lotka_volterra::Simulation sim3(dt3, A, B, C, D, x0, y0);
  sim3.evolve_time(T);

  lotka_volterra::State const& s1 = sim1.state_at(sim1.steps() - 1);
  lotka_volterra::State const& s2 = sim2.state_at(sim2.steps() - 1);
  lotka_volterra::State const& s3 = sim3.state_at(sim3.steps() - 1);

  double error1 = std::abs(s1.x - s3.x);
  double error2 = std::abs(s2.x - s3.x);

  // Ordine di convergenza p ≈ log(error1/error2)/log(dt1/dt2)
  double p = std::log(error1 / error2) / std::log(dt1 / dt2);
  CHECK(p == doctest::Approx(1.).epsilon(0.01));
}