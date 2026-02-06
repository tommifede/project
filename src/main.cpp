#include "input.hpp"
#include "output.hpp"
#include <iostream>

int main()
{
  try {
    // interactive input
    lotka_volterra::Simulation sim = io::inputSimulation();
    lotka_volterra::Renderer ren   = io::inputRenderer();
    double T                       = io::inputTime(sim);
    // direct input
    // lotka_volterra::Simulation sim = io::inputSimulation({0.001, 10., 6., 4., 12., 4., 3.});
    // lotka_volterra::Renderer ren   = io::inputRenderer(1000);
    // double T                       = 10.;
    // or
    // lotka_volterra::Simulation sim{0.001, 10., 6., 4., 12., 4., 3.};
    // lotka_volterra::Renderer ren{1000};

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; // smooth trajectory

    sf::RenderWindow win(sf::VideoMode(static_cast<unsigned int>(ren.size()), static_cast<unsigned int>(ren.size())),
                         "Lotka-Volterra Simulation", sf::Style::Titlebar | sf::Style::Close, settings);

    std::size_t step = 0;
    bool finished    = false;
    while (win.isOpen()) {
      sf::Event event;
      while (win.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          win.close();
        }
      }
      if (step < static_cast<std::size_t>(T / sim.dt())) {
        if (step + 1 >= sim.steps()) {
          sim.evolve();
          ++step;
        }
        win.clear(sf::Color::White);
        ren.draw(win, sim, step);
        win.display();
      } else if (!finished) {
        win.setTitle("Lotka-Volterra Simulation complete!");
        finished = true;
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