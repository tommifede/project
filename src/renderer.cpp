#include "../include/renderer.hpp"
#include "../include/simulation.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

namespace lotka_volterra {
sf::Vector2f Renderer::map_screen(double x, double y, double x_max,
                                  double y_max)
{
  float margin = 50.f;
  float x_screen =
      static_cast<float>(x / x_max) * (static_cast<float>(width_) - 2 * margin)
      + margin;
  float y_screen =
      static_cast<float>(height_)
      - (static_cast<float>(
          (y / y_max) * (static_cast<float>(height_) - 2 * margin) + margin));
  return {x_screen, y_screen};
}

void Renderer::check_parameters(int width, int height)
{
  if (width <= 0. || height <= 0) {
    throw std::invalid_argument("width, height must be > 0");
  }
}

sf::Color Renderer::color_energy(double H, double H0) const
{
  if (!std::isfinite(H)) {
    return sf::Color::Red;
  }

  double dH = std::abs(H - H0);
  double t  = std::min(dH / std::abs(H0), 1.0);

  sf::Uint8 r = static_cast<sf::Uint8>(255 * t);
  sf::Uint8 g = static_cast<sf::Uint8>(255 * (1 - t));

  return {r, g, 0};
}


Renderer::Renderer(int width, int height)
    : width_{width}
    , height_{height}
{
  check_parameters(width, height);
}

void Renderer::draw(sf::RenderWindow& window, Simulation const& simulation)
{
  draw(window, simulation, simulation.steps());
}

void Renderer::draw(sf::RenderWindow& window, Simulation const& simulation,
                    int current_step)
{
  if (current_step == 0) {
    return;
  }
  current_step = std::min(current_step, simulation.steps());
  double x_max = 0.;
  double y_max = 0.;
  for (int i = 0; i < current_step; ++i) {
    State const& state = simulation.state_at(i);
    x_max              = std::max(x_max, state.x);
    y_max              = std::max(y_max, state.y);
  }

  sf::VertexArray trajectory(sf::LineStrip,
                             static_cast<std::size_t>(current_step));
  double H0 = simulation.state_at(0).H;
  for (int i = 0; i < current_step; ++i) {
    State const& state = simulation.state_at(i);
    trajectory[static_cast<std::size_t>(i)].position =
        map_screen(state.x, state.y, x_max, y_max);
    trajectory[static_cast<std::size_t>(i)].color = color_energy(state.H, H0);
  }

  window.draw(trajectory);
}

void Renderer::draw_axes(sf::RenderWindow& window, int width, int height) const
{
  sf::Vertex axes[] = {
      {{50.f, static_cast<float>(height) - 50.f}, sf::Color::Black},
      {{static_cast<float>(width) - 50.f, static_cast<float>(height) - 50.f},
       sf::Color::Black},
      {{50.f, 50.f}, sf::Color::Black},
      {{50.f, static_cast<float>(height) - 50.f}, sf::Color::Black}};

  window.draw(axes, 2, sf::Lines);     // x-axis
  window.draw(axes + 2, 2, sf::Lines); // y-axis
}

void Renderer::draw_ticks(sf::RenderWindow& window, int width, int height,
                          int n) const
{
  for (int i = 1; i < n; ++i) {
    float x = 50.f
            + static_cast<float>(i) * (static_cast<float>(width) - 100.f)
                  / static_cast<float>(n);
    float y = 50.f
            + static_cast<float>(i) * (static_cast<float>(height) - 100.f)
                  / static_cast<float>(n);

    sf::Vertex tx[] = {
        {{x, static_cast<float>(height) - 45.f}, sf::Color::Black},
        {{x, static_cast<float>(height) - 55.f}, sf::Color::Black}};
    sf::Vertex ty[] = {
        {{45.f, static_cast<float>(height) - y}, sf::Color::Black},
        {{55.f, static_cast<float>(height) - y}, sf::Color::Black}};

    window.draw(tx, 2, sf::Lines);
    window.draw(ty, 2, sf::Lines);
  }
}
} // namespace lotka_volterra