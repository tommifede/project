#include "../include/input.hpp"
#include "../include/output.hpp"
#include <iostream>

int main()
{
  try {
    lotka_volterra::Simulation sim = io::input_simulation();
    // lotka_volterra::Simulation sim =
    //     io::input_simulation({0.01, 5., 1., 1., 4., 3.8, 5.3});
    lotka_volterra::Renderer ren = io::input_renderer();
    // lotka_volterra::Renderer ren = io::input_renderer({800, 800});
    double T = io::input_T(sim);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow win(
        sf::VideoMode(static_cast<unsigned int>(ren.getSide()),
                      static_cast<unsigned int>(ren.getSide())),
        "Lotka-Volterra", sf::Style::Default, settings);
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
    io::output_csv(sim, "trajectory.csv");

    return 0;
  } catch (std::exception const& e) {
    std::cerr << "Fatal error: " << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown fatal error\n";
    return EXIT_FAILURE;
  }
}