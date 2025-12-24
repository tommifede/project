#include "../include/output.hpp"
#include "../include/renderer.hpp"
#include "../include/simulation.hpp"
#include <SFML/Graphics.hpp>
#include <string>

int main()
{
  int const width  = 800;
  int const height = 600;

  /* sf::RenderWindow window1(sf::VideoMode(width, height), "Lotka-Volterra 1");

  lotka_volterra::Simulation simulation1(0.0001, 2., 0.1, 0.1, 1., 5.5, 10.);
  simulation1.evolve_time(100.);

  io::output_csv(simulation1, "trajectory1.csv");

  lotka_volterra::Renderer renderer1(width, height);
  while (window1.isOpen()) {
    sf::Event event;
    while (window1.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window1.close();
      }
    }
    window1.clear(sf::Color::White);
    renderer1.draw_axes(window1, width, height);
    renderer1.draw_ticks(window1, width, height);
    renderer1.draw(window1, simulation1);
    window1.display();
  } */

  sf::RenderWindow window2(sf::VideoMode(width, height), "Lotka-Volterra 2");

  lotka_volterra::Simulation simulation2(0.01, 2., 0.1, 0.1, 1., 8, 10.);

  int step = 0;
  lotka_volterra::Renderer renderer2(width, height);
  while (window2.isOpen()) {
    sf::Event event;
    while (window2.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window2.close();
    }

    if (step + 1 < simulation2.steps())
      ++step;
    else
      simulation2.evolve();

    window2.clear(sf::Color::White);
    renderer2.draw_axes(window2, width, height);
    renderer2.draw_ticks(window2, width, height);
    renderer2.draw(window2, simulation2, step);
    window2.display();
  }
  io::output_csv(simulation2, "trajectory2.csv");

  return 0;
}