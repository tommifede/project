#include "doctest.h"

#include "../include/renderer.hpp"

TEST_CASE("Renderer constructor rejects width and height out of range")
{
  CHECK_THROWS(lotka_volterra::Renderer(700));
  CHECK_THROWS(lotka_volterra::Renderer(1100));
  CHECK_NOTHROW(lotka_volterra::Renderer(800));
  CHECK_NOTHROW(lotka_volterra::Renderer(1000));
}

TEST_CASE("Renderer draws without crashing")
{
  lotka_volterra::Simulation sim(0.01, 2., 0.1, 0.1, 1., 10., 10.);
  sim.evolve_steps(10);

  lotka_volterra::Renderer r(800);
  sf::RenderWindow window(sf::VideoMode(800, 800), "test", sf::Style::None);

  CHECK_NOTHROW(r.draw(window, sim));
}

TEST_CASE("Renderer draws by step without crashing")
{
  lotka_volterra::Simulation sim(0.01, 2., 0.1, 0.1, 1., 10., 10.);
  sim.evolve_steps(5);

  lotka_volterra::Renderer r(800);
  sf::RenderWindow window(sf::VideoMode(800, 800), "test", sf::Style::None);

  CHECK_NOTHROW(r.draw(window, sim, 3));
  CHECK_NOTHROW(r.draw(window, sim, 5));
  CHECK_NOTHROW(r.draw(window, sim, 6));
}

TEST_CASE("Renderer draws equilibrium point and draws axes without crashing")
{
  lotka_volterra::Renderer r(800);
  sf::RenderWindow window(sf::VideoMode(800, 800), "test", sf::Style::None);
  sf::View view;

  CHECK_NOTHROW(r.drawEqPoint(window, view));
  CHECK_NOTHROW(r.drawAxes(window, view));
}
