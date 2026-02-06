#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "simulation.hpp"
#include <SFML/Graphics.hpp>

namespace lotka_volterra {
class Renderer
{
private:
  std::size_t size_;
  double world_max_;
  double pixels_per_unit_;
  float y0_;
  double tick_step_;
  double x_eq_;
  double y_eq_;
  double max_x_;
  double max_y_;
  float axis_offset_;
  float axis_length_;
  sf::VertexArray trajectory_{sf::LineStrip};
  sf::Text label_;
  sf::Font font_;
  std::size_t last_drawn_step_  = 0;
  int tick_count_               = 10;
  unsigned int label_font_size_ = 12;
  float eq_point_radius_        = 5.f;
  bool set_                     = false;

  void check_parameter(std::size_t size);
  void check_set();
  sf::Color color_energy(double H, double H0) const;
  double compute_tick_step(double max_value);
  double step_tick(double margin, double max_x, double max_y);
  double max_world(double margin, double max_x, double max_y);
  double scale(double world_max, float axis_offset, sf::View ui_view);
  void update_trajectory(Simulation const& simulation, std::size_t current_step);
  void validate_window(sf::RenderWindow const& window) const;

public:
  Renderer(std::size_t size);
  std::size_t size() const;
  void setDraw(sf::RenderWindow& window, Simulation const& simulation, std::size_t current_step, sf::View const& ui_view,
               sf::View& world_view, double margin = 1.2, float axis_offset = 100.f);
  void drawAxes(sf::RenderWindow& window, sf::View const& ui_view);
  void drawTicks(sf::RenderWindow& window, sf::View const& ui_view);
  void drawTrajectory(sf::RenderWindow& window, Simulation const& simulation, std::size_t current_step, sf::View const& world_view);
  void drawEqPoints(sf::RenderWindow& window, sf::View const& ui_view, sf::View const& world_view);
  void drawTitles(sf::RenderWindow& window, sf::View const& ui_view);
  void draw(sf::RenderWindow& window, Simulation const& simulation, std::size_t current_step);
  void draw(sf::RenderWindow& window, Simulation const& simulation);
};
} // namespace lotka_volterra

#endif