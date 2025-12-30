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
  double world_max_;
  double pixels_per_unit_;
  float y0_;
  double tick_step_;
  double x_eq_;
  double y_eq_;
  double max_x_;
  double max_y_;
  float axis_offset_;
  sf::VertexArray trajectory_;
  std::size_t last_step_drawn_ = 0;
  sf::Text label_;
  sf::Font font_;
  int tick_count_               = 10;
  unsigned int label_font_size_ = 12;
  float eq_point_radius_        = 5.f;
  double world_margin_factor_   = 1.05;

  void check_parameters(std::size_t width, std::size_t height);
  sf::Color color_energy(double H, double H0) const;
  double compute_tick_step(double max_value);
  double step_tick(double margin, double max_x, double max_y);
  double max_world(double margin, double max_x, double max_y);
  double scale(double world_max, float axis_offset, sf::View uiView);
  void update_trajectory(Simulation const& sim, std::size_t current_step);

public:
  Renderer(std::size_t width, std::size_t height);
  void setDraw(sf::RenderWindow& window, Simulation const& simulation,
               std::size_t current_step, sf::View uiView, sf::View& worldView,
               double margin = 1.05, float axis_offset = 40.f);
  void draw_axes(sf::RenderWindow& window, sf::View uiView);
  void draw_ticks(sf::RenderWindow& window, sf::View uiView);
  void draw_trajectory(sf::RenderWindow& window, Simulation const& simulation,
                       std::size_t current_step, sf::View& worldView);
  void draw_eq_point(sf::RenderWindow& window, sf::View uiView,
                     sf::View worldView);
  void draw(sf::RenderWindow& window, Simulation const& simulation,
            std::size_t current_step);
  void draw(sf::RenderWindow& window, Simulation const& simulation);
};
} // namespace lotka_volterra

#endif