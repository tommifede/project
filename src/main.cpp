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
    std::size_t const max_steps    = static_cast<std::size_t>(T / sim.dt());
    // direct input
    // lotka_volterra::Simulation sim = io::inputSimulation({0.001, 10., 6., 4., 12., 4., 3.});
    // lotka_volterra::Renderer ren   = io::inputRenderer(1000);
    // double T                       = 10.;
    // std::size_t const max_steps    = static_cast<std::size_t>(std::min(T / sim.dt(), 1e7)); // total simulation steps
    // or
    // lotka_volterra::Simulation sim{0.001, 10., 6., 4., 12., 4., 3.};
    // lotka_volterra::Renderer ren{1000};
    // double T                    = 10.;
    // std::size_t const max_steps = static_cast<std::size_t>(std::min(T / sim.dt(), 1e7)); // total simulation steps

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; // smooth trajectory

    sf::RenderWindow win(sf::VideoMode(static_cast<unsigned int>(ren.size()), static_cast<unsigned int>(ren.size())),
                         "Lotka-Volterra Simulation", sf::Style::Titlebar | sf::Style::Close, settings);
    win.setFramerateLimit(240); // max frame rate

    std::size_t frame = 0; // renderer frame
    bool title_set    = false;

    while (win.isOpen()) {
      sf::Event event;
      while (win.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          win.close();
          std::cout << "\nSimulation finished\n";
          std::cout << "Total steps performed: " << sim.steps() << "\n";
          std::cout << "Status: ABORTED\n";

          io::outputCSV(sim, "trajectory.csv");
          return 0;
        }
      }

      win.clear(sf::Color::White);
      if (!sim.isUnstable() && sim.steps() < max_steps && sim.evolve()) {
        std::cout << "dt " << static_cast<double>(sim.steps()) * sim.dt() << " | H = " << sim.H() << "\n";
        ren.draw(win, sim, frame);
        ++frame;
      } else {
        ren.draw(win, sim, sim.steps() - 1); // draw last valid state (avoid white window)

        if (!title_set) {
          if (sim.isUnstable())
            win.setTitle("Lotka-Volterra Simulation aborted");
          else
            win.setTitle("Lotka-Volterra Simulation complete");

          title_set = true;
        }
      }

      win.display();
    }

    io::outputStatus(sim);
    io::outputCSV(sim, "trajectory.csv");

    return 0;
  } catch (std::exception const& e) {
    std::cerr << "Fatal error: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown fatal error.\n";
    return EXIT_FAILURE;
  }
}