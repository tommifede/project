#include "../include/renderer.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <format>

namespace lotka_volterra {
void Renderer::check_parameter(std::size_t size)
{
  if (size
      != std::clamp(size, static_cast<std::size_t>(800),
                    static_cast<std::size_t>(1000))) {
    throw std::invalid_argument("size must be between 800 and 1000.");
  }
}

sf::Color Renderer::color_energy(double H, double H0) const
{
  if (!std::isfinite(H)) {
    return sf::Color::Red;
  }

  double dH = std::abs(H - H0);
  double t  = (H0 != 0) ? std::clamp(dH / std::abs(H0), 0., 1.) : 1.;

  sf::Uint8 r;
  sf::Uint8 g;
  if (t < 0.5) {
    r = static_cast<sf::Uint8>(t * 2 * 255);
    g = 255;
  } else {
    r = 255;
    g = static_cast<sf::Uint8>((1 - (t - 0.5) * 2) * 255);
  }

  return {r, g, 0};
}

double Renderer::compute_tick_step(double max_value)
{
  if (max_value <= 0.) {
    return 1.;
  }

  double raw_step = max_value / tick_count_;
  double order    = std::floor(std::log10(raw_step));
  double pow_step = std::pow(10., order);
  double factor   = raw_step / pow_step;

  if (factor < 2.) {
    return 1. * pow_step;
  }
  if (factor < 5.) {
    return 2. * pow_step;
  }

  return 5. * pow_step;
}

double Renderer::step_tick(double margin, double max_x, double max_y)
{
  double worldExtent = std::max(max_x, max_y) * margin;

  return compute_tick_step(worldExtent);
}

double Renderer::max_world(double margin, double max_x, double max_y)
{
  double worldExtent = std::max(max_x, max_y) * margin;
  double tick_step   = compute_tick_step(worldExtent);
  double num_ticks   = std::ceil(worldExtent / tick_step);

  return num_ticks * tick_step;
}

double Renderer::scale(double world_max, float axis_offset, sf::View ui_view)
{
  float drawableWidth  = ui_view.getSize().x - 2 * axis_offset;
  float drawableHeight = ui_view.getSize().y - 2 * axis_offset;

  return std::min(drawableWidth / static_cast<float>(world_max),
                  drawableHeight / static_cast<float>(world_max));
}

void Renderer::update_trajectory(Simulation const& simulation,
                                 std::size_t current_step)
{
  double H0 = simulation.stateAt(0).H;

  for (std::size_t i = last_step_drawn_; i < current_step; ++i) {
    State const& state = simulation.stateAt(i);
    trajectory_.append(
        {{static_cast<float>(state.x), static_cast<float>(state.y)},
         color_energy(state.H, H0)});
  }

  last_step_drawn_ = current_step;
}


Renderer::Renderer(std::size_t size)
    : size_{size}
{
  check_parameter(size);

  if (!font_.loadFromFile("font.ttf")) {
    throw std::runtime_error("failed to load font.");
  }
}

std::size_t Renderer::getSize() const
{
  return size_;
}

void Renderer::setDraw(sf::RenderWindow& window, Simulation const& simulation,
                       std::size_t current_step, sf::View const& ui_view,
                       sf::View& world_view, double margin, float axis_offset)
{
  x_eq_  = simulation.getParameter(3) / simulation.getParameter(2);
  y_eq_  = simulation.getParameter(0) / simulation.getParameter(1);
  max_x_ = x_eq_;
  max_y_ = y_eq_;

  for (std::size_t i = 0; i < current_step; ++i) {
    State const& state = simulation.stateAt(i);
    max_x_             = std::max(max_x_, std::abs(state.x));
    max_y_             = std::max(max_y_, std::abs(state.y));
  }

  world_max_       = max_world(margin, max_x_, max_y_);
  pixels_per_unit_ = scale(world_max_, axis_offset, ui_view);
  y0_              = ui_view.getSize().y - axis_offset;
  tick_step_       = step_tick(margin, max_x_, max_y_);
  axis_offset_     = axis_offset;

  label_.setFont(font_);
  label_.setCharacterSize(label_font_size_);
  label_.setFillColor(sf::Color::Black);

  float world_size = static_cast<float>(world_max_);
  world_view.setSize(world_size, -world_size);
  world_view.setCenter(world_size / 2.f, world_size / 2.f);

  sf::Vector2u winSize = window.getSize();
  float left           = axis_offset_ / static_cast<float>(winSize.x);
  float bottom         = axis_offset_ / static_cast<float>(winSize.y);

  world_view.setViewport({left, bottom, 1.f - 2 * left, 1.f - 2 * bottom});
}


void Renderer::drawAxes(sf::RenderWindow& window, sf::View const& ui_view)
{
  window.setView(ui_view);

  sf::Vertex x_axis[] = {
      {{axis_offset_, y0_}, sf::Color::Black},
      {{axis_offset_ + static_cast<float>(world_max_ * pixels_per_unit_), y0_},
       sf::Color::Black}};

  sf::Vertex y_axis[] = {
      {{axis_offset_, y0_}, sf::Color::Black},
      {{axis_offset_,
        std::max(y0_ - static_cast<float>(world_max_ * pixels_per_unit_),
                 axis_offset_)},
       sf::Color::Black}};

  window.draw(x_axis, 2, sf::Lines);
  window.draw(y_axis, 2, sf::Lines);
}

void Renderer::drawTicks(sf::RenderWindow& window, sf::View const& ui_view)
{
  window.setView(ui_view);

  sf::Color tick_color = sf::Color::Black;
  sf::Color grid_color = sf::Color(200, 200, 200, 100);

  int precision =
      std::max(0, -static_cast<int>(std::floor(std::log10(tick_step_))));
  std::string format = "{:." + std::to_string(precision) + "f}";

  for (double x = 0.; x <= world_max_; x += tick_step_) {
    float px          = axis_offset_ + static_cast<float>(x * pixels_per_unit_);
    sf::Vertex tick[] = {{{px, y0_ - 5.f}, tick_color},
                         {{px, y0_ + 5.f}, tick_color}};
    window.draw(tick, 2, sf::Lines);

    std::string label = std::vformat(format, std::make_format_args(x));
    label_.setString(label);
    sf::FloatRect bounds = label_.getLocalBounds();
    label_.setPosition(px - bounds.width / 2.f, y0_ + 8.f);
    window.draw(label_);

    sf::Vertex grid[] = {
        {{px, y0_}, grid_color},
        {{px, axis_offset_}},
    };
    window.draw(grid, 2, sf::Lines);
  }

  for (double y = 0.; y <= world_max_; y += tick_step_) {
    float py = y0_ - static_cast<float>(y * pixels_per_unit_);
    if (py < axis_offset_) {
      break;
    }
    sf::Vertex tick[] = {{{axis_offset_ - 5.f, py}, tick_color},
                         {{axis_offset_ + 5.f, py}, tick_color}};

    window.draw(tick, 2, sf::Lines);

    std::string label = std::vformat(format, std::make_format_args(y));
    label_.setString(label);
    sf::FloatRect bounds = label_.getLocalBounds();
    label_.setPosition(axis_offset_ - bounds.width - 10.f, py - bounds.height);

    window.draw(label_);

    sf::Vertex grid[] = {
        {{axis_offset_, py}, grid_color},
        {{axis_offset_ + static_cast<float>(world_max_ * pixels_per_unit_), py},
         grid_color}};

    window.draw(grid, 2, sf::Lines);
  }
}

void Renderer::drawTrajectory(sf::RenderWindow& window,
                              Simulation const& simulation,
                              std::size_t current_step,
                              sf::View const& world_view)
{
  window.setView(world_view);
  update_trajectory(simulation, current_step);

  window.draw(trajectory_);
}

void Renderer::drawEqPoints(sf::RenderWindow& window, sf::View const& ui_view,
                            sf::View const& world_view)
{
  window.setView(world_view);

  float radius_world = eq_point_radius_ / static_cast<float>(pixels_per_unit_);
  sf::CircleShape eq_point(radius_world);
  eq_point.setFillColor(sf::Color::Blue);
  eq_point.setOrigin(radius_world, radius_world);
  eq_point.setPosition({static_cast<float>(x_eq_), static_cast<float>(y_eq_)});

  window.draw(eq_point);

  window.setView(ui_view);

  sf::CircleShape origin_eq_point(eq_point_radius_);
  origin_eq_point.setFillColor(sf::Color::Cyan);
  origin_eq_point.setOrigin(eq_point_radius_, eq_point_radius_);
  origin_eq_point.setPosition({axis_offset_, y0_});

  window.draw(origin_eq_point);
}

void Renderer::drawTitles(sf::RenderWindow& window, sf::View const& ui_view)
{
  window.setView(ui_view);

  sf::Text title;
  title.setFont(font_);
  title.setString("LOTKA-VOLTERRA SIMULATION RENDERING");
  title.setCharacterSize(static_cast<unsigned int>(axis_offset_ / 3.f));
  title.setFillColor(sf::Color::Black);

  sf::FloatRect bounds = title.getLocalBounds();
  title.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  title.setPosition(ui_view.getSize().x / 2.f, axis_offset_ / 2.f);

  window.draw(title);

  sf::Text x_title;
  x_title.setFont(font_);
  x_title.setString("prey");
  x_title.setCharacterSize(static_cast<unsigned int>(axis_offset_ / 4.f));
  x_title.setFillColor(sf::Color::Black);

  sf::FloatRect x_bounds = x_title.getLocalBounds();
  x_title.setOrigin(x_bounds.width / 2.f, x_bounds.height / 2.f);
  x_title.setPosition(
      axis_offset_ + static_cast<float>(world_max_ * pixels_per_unit_) / 2.f,
      ui_view.getSize().y - axis_offset_ / 2.f);

  window.draw(x_title);


  sf::Text y_title;
  y_title.setFont(font_);
  y_title.setString("predator");
  y_title.setCharacterSize(static_cast<unsigned int>(axis_offset_ / 4.f));
  y_title.setFillColor(sf::Color::Black);

  sf::FloatRect y_bounds = y_title.getLocalBounds();
  y_title.setOrigin(y_bounds.width / 2.f, y_bounds.height / 2.f);
  y_title.setRotation(-90.f);
  y_title.setPosition(
      axis_offset_ / 3.f,
      y0_ - static_cast<float>(world_max_ * pixels_per_unit_) / 2.f);

  window.draw(y_title);
}

void Renderer::draw(sf::RenderWindow& window, Simulation const& simulation,
                    std::size_t current_step)
{
  if (current_step == 0) {
    return;
  }

  current_step = std::min(current_step, simulation.steps());

  sf::View world_view;
  sf::View ui_view = window.getDefaultView();

  setDraw(window, simulation, current_step, ui_view, world_view);
  drawTicks(window, ui_view);
  drawAxes(window, ui_view);
  drawTrajectory(window, simulation, current_step, world_view);
  drawTitles(window, ui_view);
  drawEqPoints(window, ui_view, world_view);
}

void Renderer::draw(sf::RenderWindow& window, Simulation const& simulation)
{
  draw(window, simulation, simulation.steps());
}
} // namespace lotka_volterra
