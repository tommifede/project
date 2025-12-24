#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "simulation.hpp"
#include <SFML/Graphics.hpp>

namespace lotka_volterra {
class Renderer
{
private:
  int width_;
  int height_;

  sf::Vector2f map_screen(double x, double y, double x_max, double y_max);
  void check_parameters(int width, int height);
  sf::Color color_energy(double H, double H0) const;

public:
  Renderer(int width, int height);
  void draw(sf::RenderWindow& window, Simulation const& simulation);
  void draw(sf::RenderWindow& window, Simulation const& simulation,
            int current_step);
  void draw_axes(sf::RenderWindow& window, int width, int height) const;
  void draw_ticks(sf::RenderWindow& window, int width, int height,
                  int n = 10) const;
};
} // namespace lotka_volterra

#endif