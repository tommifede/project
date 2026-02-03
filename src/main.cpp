#include "input.hpp"
#include "output.hpp"
#include <iostream>

int main()
{
  try {
    lotka_volterra::Simulation sim = io::inputSimulation();
    // lotka_volterra::Simulation sim =
    // io::inputSimulation({0.001, 10., 6., 4., 12., 4., 3.});
    lotka_volterra::Renderer ren = io::inputRenderer();
    // lotka_volterra::Renderer ren = io::inputRenderer(1000);
    double T = io::inputTime(sim);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow win(
        sf::VideoMode(static_cast<unsigned int>(ren.getSize()),
                      static_cast<unsigned int>(ren.getSize())),
        "Lotka-Volterra", sf::Style::Titlebar | sf::Style::Close, settings);
    std::size_t step = 0;

    while (win.isOpen()) {
      sf::Event event;
      while (win.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          win.close();
        }
      }
      if (step < static_cast<std::size_t>(T / sim.dt())) {
        if (step + 1 < sim.steps()) {
          ++step;
        } else {
          sim.evolve();
        }
        win.clear(sf::Color::White);
        ren.draw(win, sim, step);
        win.display();
      }
    }
    
    io::outputCSV(sim, "trajectory.csv");

    return 0;
  } catch (std::exception const& e) {
    std::cerr << "Fatal error: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown fatal error\n";
    return EXIT_FAILURE;
  }
}