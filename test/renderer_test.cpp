#include "doctest.h"

#include "../include/renderer.hpp"
#include "../include/simulation.hpp"
#include <SFML/Graphics.hpp>

TEST_CASE("Renderer constructor rejects zero size")
{
  CHECK_THROWS(lotka_volterra::Renderer(0, 100));
  CHECK_THROWS(lotka_volterra::Renderer(100, 0));
}

TEST_CASE("Renderer draws without crashing")
{
  lotka_volterra::Simulation sim(0.01, 2., 0.1, 0.1, 1., 10., 10.);
  sim.evolve_steps(10);

  lotka_volterra::Renderer r(200, 200);
  sf::RenderWindow window(sf::VideoMode(200, 200), "test", sf::Style::None);

  CHECK_NOTHROW(r.draw(window, sim));
}

TEST_CASE("Renderer draws by step without crashing")
{
  lotka_volterra::Simulation sim(0.01, 2., 0.1, 0.1, 1., 10., 10.);
  sim.evolve_steps(5);

  lotka_volterra::Renderer r(200, 200);
  sf::RenderWindow window(sf::VideoMode(200, 200), "test", sf::Style::None);

  CHECK_NOTHROW(r.draw(window, sim, 3));
  CHECK_NOTHROW(r.draw(window, sim, 5));
  CHECK_NOTHROW(r.draw(window, sim, 6));
}

TEST_CASE("Renderer draws equilibrium point and draws axes without crashing")
{
  lotka_volterra::Renderer r(200, 200);
  sf::RenderWindow window(sf::VideoMode(200, 200), "test", sf::Style::None);
  sf::View view;
  view.setSize(200.f, 200.f);

  CHECK_NOTHROW(r.draw_eq_point(window, view, view));
  CHECK_NOTHROW(r.draw_axes(window, view));
}

