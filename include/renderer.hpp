#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "simulation.hpp"
#include <SFML/Graphics.hpp>

namespace lotka_volterra {
class Renderer
{
private:
  std::size_t width_;
  std::size_t height_;

  sf::Vector2f map_screen(double x, double y, double x_eq, double y_eq,
                          double scale);
  void check_parameters(std::size_t width, std::size_t height);
  sf::Color color_energy(double H, double H0) const;

public:
  Renderer(std::size_t width, std::size_t height);
  void draw(sf::RenderWindow& window, Simulation const& simulation);
  void draw(sf::RenderWindow& window, Simulation const& simulation,
            std::size_t current_step);
  /*   void draw_axes(sf::RenderWindow& window, std::size_t width,
                   std::size_t height) const;
    void draw_ticks(sf::RenderWindow& window, std::size_t width,
                    std::size_t height, int n = 10) const; */
};
} // namespace lotka_volterra

#endif