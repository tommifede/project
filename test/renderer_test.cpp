#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "input.hpp"

TEST_CASE("Renderer constructor rejects width and height out of range")
{
  CHECK_THROWS(lotka_volterra::Renderer(700));
  CHECK_THROWS(lotka_volterra::Renderer(1100));
  CHECK_NOTHROW(lotka_volterra::Renderer(800));
  CHECK_NOTHROW(lotka_volterra::Renderer(1000));
}

TEST_CASE("Renderer draws without crashing")
{
  lotka_volterra::Simulation sim{0.001, 1., 1., 1., 1., 1., 1.};
  sim.evolveSteps(10);

  lotka_volterra::Renderer r{800};
  sf::RenderWindow window{sf::VideoMode(800, 800), "test", sf::Style::None};

  CHECK_NOTHROW(r.draw(window, sim));
}

TEST_CASE("Renderer draws by step without crashing")
{
  lotka_volterra::Simulation sim{0.001, 1., 1., 1., 1., 1., 1.};
  sim.evolveSteps(5);

  lotka_volterra::Renderer r{800};
  sf::RenderWindow window{sf::VideoMode(800, 800), "test", sf::Style::None};

  CHECK_NOTHROW(r.draw(window, sim, 3));
  CHECK_NOTHROW(r.draw(window, sim, 5));
  CHECK_NOTHROW(r.draw(window, sim, 6));
}

TEST_CASE("Renderer sets draw and draws components without crashing")
{
  lotka_volterra::Renderer r{800};
  sf::RenderWindow window{sf::VideoMode(800, 800), "test", sf::Style::None};
  sf::View view;
  lotka_volterra::Simulation s{0.01, 2., 0.1, 0.1, 1., 10., 10.};
  CHECK_NOTHROW(r.setDraw(window, s, 1, view, view));
  CHECK_NOTHROW(r.drawEqPoints(window, view, view));
  CHECK_NOTHROW(r.drawAxes(window, view));
  CHECK_NOTHROW(r.drawTicks(window, view));
  CHECK_NOTHROW(r.drawTitles(window, view));
  CHECK_NOTHROW(r.drawTrajectory(window, s, 1, view));
}

TEST_CASE("Renderer input creates a proper renderer")
{
  lotka_volterra::Renderer r = io::inputRenderer(800);
  CHECK(r.size() == 800);

  CHECK_THROWS(io::inputRenderer(700));
}