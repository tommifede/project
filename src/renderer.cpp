#include "../include/renderer.hpp"
#include "../include/simulation.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

namespace lotka_volterra {
sf::Vector2f Renderer::map_screen(double x, double y, double x_eq, double y_eq,
                                  double scale)
{
  float x_screen =
      static_cast<float>(width_) / 2.f + static_cast<float>((x - x_eq) * scale);
  float y_screen = static_cast<float>(height_) / 2.f
                 - static_cast<float>((y - y_eq) * scale);
  return {x_screen, y_screen};
}

void Renderer::check_parameters(std::size_t width, std::size_t height)
{
  if (width <= 0 || height <= 0) {
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


Renderer::Renderer(std::size_t width, std::size_t height)
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
                    std::size_t current_step)
{
  if (current_step == 0) {
    return;
  }
  current_step = std::min(current_step, simulation.steps());
  double x_eq  = simulation.GetPar(3) / simulation.GetPar(2);
  double y_eq  = simulation.GetPar(0) / simulation.GetPar(1);
  double max_x = std::abs(x_eq);
  double max_y = std::abs(y_eq);
  for (std::size_t i = 0; i < current_step; ++i) {
    State const& state = simulation.state_at(i);
    max_x              = std::max(max_x, std::abs(state.x));
    max_y              = std::max(max_y, std::abs(state.y));
  }

  sf::View view;
  double margin      = 1.1;
  double axis_margin = 1.5;
  view.setCenter(static_cast<float>((max_x * margin - axis_margin) / 2),
                 static_cast<float>(max_y * margin - axis_margin) / 2);

  view.setSize(static_cast<float>(max_x * margin + axis_margin),
               static_cast<float>(-(max_y * margin + axis_margin)));

  window.setView(view);
  window.setView(view);

  sf::Vertex x_axis[] = {{{0.f, 0.f}, sf::Color::Black},
                         {{1e6f, 0.f}, sf::Color::Black}};
  sf::Vertex y_axis[] = {{{0.f, 0.f}, sf::Color::Black},
                         {{0.f, 1e6f}, sf::Color::Black}};
  window.draw(x_axis, 2, sf::Lines);
  window.draw(y_axis, 2, sf::Lines);

  double tick_step = 1.5;
  for (double x = -max_x; x <= max_x; x += tick_step) {
    sf::Vertex tick[] = {{{static_cast<float>(x), -0.1f}, sf::Color::Black},
                         {{static_cast<float>(x), 0.1f}, sf::Color::Black}};
    window.draw(tick, 2, sf::Lines);
  }

  for (double y = -max_y; y <= max_y; y += tick_step) {
    sf::Vertex tick[] = {{{-0.05f, static_cast<float>(y)}, sf::Color::Black},
                         {{0.05f, static_cast<float>(y)}, sf::Color::Black}};
    window.draw(tick, 2, sf::Lines);
  }

  sf::VertexArray trajectory(sf::LineStrip, current_step);
  double H0 = simulation.state_at(0).H;
  for (std::size_t i = 0; i < current_step; ++i) {
    State const& state     = simulation.state_at(i);
    trajectory[i].position = {static_cast<float>(state.x),
                              static_cast<float>(state.y)};
    trajectory[i].color    = color_energy(state.H, H0);
  }
  window.draw(trajectory);

  sf::View worldView = window.getView();
  window.setView(window.getDefaultView());

  sf::Vector2f eq_world(static_cast<float>(x_eq), static_cast<float>(y_eq));
  sf::Vector2f eq_screen =
      static_cast<sf::Vector2f>(window.mapCoordsToPixel(eq_world, worldView));
  sf::CircleShape eq_point(5.f); // 5 pixel
  eq_point.setFillColor(sf::Color::Blue);
  eq_point.setOrigin(5.f, 5.f);
  eq_point.setPosition(eq_screen);
  window.draw(eq_point);

  window.setView(worldView);
}

/* void Renderer::draw_axes(sf::RenderWindow& window, std::size_t width,
                         std::size_t height) const
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

void Renderer::draw_ticks(sf::RenderWindow& window, std::size_t width,
                          std::size_t height, int n) const
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
} */
} // namespace lotka_volterra