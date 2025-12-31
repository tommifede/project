#include "../include/input.hpp"
#include "../include/output.hpp"
#include "../include/renderer.hpp"
#include "../include/simulation.hpp"
#include <SFML/Graphics.hpp>
#include <exception>
#include <iostream>
#include <string>

int main()
{
  try {
    int const width  = 1000;
    int const height = 1000;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    lotka_volterra::Simulation simulation1(0.01, 2., 0.1, 0.1, 1., 9., 20.);
    // lotka_volterra::Simulation simulation1(0.01, 2., 0.1, 0.1, 1., 7., 15.);
    // lotka_volterra::Simulation simulation1(0.01, 2., 0.1, 0.1, 1., 4., 8.);
    simulation1.evolve_time(100.);
    lotka_volterra::Renderer renderer1(width, height);

    sf::RenderWindow window2(sf::VideoMode(width, height),
                             "Lotka-Volterra (animated)", sf::Style::Default,
                             settings);
    lotka_volterra::Simulation simulation2(0.01, 2., 0.1, 0.1, 1., 9., 20.);
    // lotka_volterra::Simulation simulation2(0.01, 2., 0.1, 0.1, 1., 7., 15.);
    // lotka_volterra::Simulation simulation2(0.01, 2., 0.1, 0.1, 1., 4., 8.);
    lotka_volterra::Renderer renderer2(width, height);
    std::size_t step2 = 0;

    while (window2.isOpen()) {
      sf::Event event2;
      while (window2.pollEvent(event2)) {
        if (event2.type == sf::Event::Closed) {
          window2.close();
        }
      }
      if (step2 < simulation1.steps()) {
        if (step2 + 1 < simulation2.steps()) {
          ++step2;
        } else {
          simulation2.evolve();
        }
        window2.clear(sf::Color::White);
        renderer2.draw(window2, simulation2, step2);
        window2.display();
      } else {
        window2.close();
        sf::RenderWindow window1(sf::VideoMode(width, height),
                                 "Lotka-Volterra (end)", sf::Style::Default,
                                 settings);
        while (window1.isOpen()) {
          sf::Event event1;
          while (window1.pollEvent(event1)) {
            if (event1.type == sf::Event::Closed) {
              window1.close();
            }
          }
          window1.clear(sf::Color::White);
          renderer1.draw(window1, simulation1);
          window1.display();
        }
      }
    }
    io::output_csv(simulation2, "trajectory2.csv");
    io::output_csv(simulation1, "trajectory1.csv");

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown fatal error\n";
    return EXIT_FAILURE;
  }
}