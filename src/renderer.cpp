#include "renderer.hpp"
#include <cmath>
#include <format>

namespace lotka_volterra {
void Renderer::check_parameter(std::size_t size) const
{
  constexpr std::size_t min_size = 800;  
  constexpr std::size_t max_size = 1000; 
  if (size != std::clamp(size, min_size, max_size)) {
    throw std::invalid_argument("parameter size must be between 800 and 1000.");
  }
}

void Renderer::check_set() const
{
  if (set_ != true) {
    throw std::logic_error("renderer draw not set.");
  }
}

void Renderer::validate_window(sf::RenderWindow const& window) const
{
  auto const size = window.getSize();
  if (size.x != size.y) {
    throw std::logic_error("renderer requires a square window.");
  }
}

sf::Color Renderer::color_energy(double H, double H0) const
{
  if (!std::isfinite(H)) {
    return sf::Color::Red;
  }

  double const dH = std::abs(H - H0);                                       
  double const t  = (H0 != 0) ? std::clamp(dH / std::abs(H0), 0., 1.) : 1.; 

  sf::Uint8 r, g;
  if (t < 0.5) { 
    r = static_cast<sf::Uint8>(t * 2 * 255);
    g = 255;
  } else { 
    r = 255;
    g = static_cast<sf::Uint8>((1 - (t - 0.5) * 2) * 255);
  }

  return {r, g, 0};
}

double Renderer::compute_tick_step(double max_value) const
{
  if (max_value <= 0.) {
    return 1.; 
  }

  double const raw_step = max_value / tick_count_;
  double const order    = std::floor(std::log10(raw_step));
  double const pow_step = std::pow(10., order);
  double const factor   = raw_step / pow_step; 

  if (factor < 2.) {
    return 1. * pow_step;
  }

  if (factor < 5.) {
    return 2. * pow_step;
  }

  return 5. * pow_step;
}

double Renderer::step_tick(double margin, double max_x, double max_y) const
{
  double worldExtent = std::max(max_x, max_y) * margin; 

  return compute_tick_step(worldExtent);
}

double Renderer::max_world(double margin, double max_x, double max_y) const
{
  double const worldExtent = std::max(max_x, max_y) * margin; 
  double const tick_step   = compute_tick_step(worldExtent);
  double const num_ticks   = std::ceil(worldExtent / tick_step); 

  return num_ticks * tick_step; 
}

double Renderer::scale(double world_max, float axis_offset, sf::View const& ui_view) const
{
  float drawableSize = ui_view.getSize().x - 2 * axis_offset; 

  return drawableSize / static_cast<float>(world_max); 
}

void Renderer::update_trajectory(Simulation const& simulation, std::size_t current_step)
{
  double const H0 = simulation.stateAt(0).H; 

  for (std::size_t i = last_drawn_step_; i < current_step; ++i) { 
    State const& state = simulation.stateAt(i);
    trajectory_.append({{static_cast<float>(state.x), static_cast<float>(state.y)}, color_energy(state.H, H0)});
  }

  last_drawn_step_ = current_step;
}

Renderer::Renderer(std::size_t size)
    : size_{size}
{
  check_parameter(size);

  if (!font_.loadFromFile("font.ttf")) {
    throw std::runtime_error("failed to load font.");
  }
}

std::size_t Renderer::size() const
{
  return size_;
}

void Renderer::setDraw(sf::RenderWindow& window, Simulation const& simulation, std::size_t current_step, sf::View const& ui_view,
                       sf::View& world_view, double margin, float axis_offset)
{
  x_eq_  = simulation.getParameter(3) / simulation.getParameter(2);
  y_eq_  = simulation.getParameter(0) / simulation.getParameter(1);
  max_x_ = x_eq_;
  max_y_ = y_eq_;

  for (std::size_t i = 0; i < current_step; ++i) {
    State const& state = simulation.stateAt(i);
    max_x_             = std::max(max_x_, state.x);
    max_y_             = std::max(max_y_, state.y);
  } 

  world_max_       = max_world(margin, max_x_, max_y_);       
  pixels_per_unit_ = scale(world_max_, axis_offset, ui_view); 
  y0_              = ui_view.getSize().y - axis_offset;       
  tick_step_       = step_tick(margin, max_x_, max_y_);
  axis_offset_     = axis_offset;
  axis_length_     = static_cast<float>(world_max_ * pixels_per_unit_);

  label_.setFont(font_);
  label_.setCharacterSize(label_font_size_);
  label_.setFillColor(sf::Color::Black);

  float const world_size = static_cast<float>(world_max_); 
  world_view.setSize(world_size, -world_size);             
  world_view.setCenter(world_size / 2.f, world_size / 2.f);

  float const side = axis_offset_ / static_cast<float>(window.getSize().x);
  world_view.setViewport({side, side, 1.f - 2 * side, 1.f - 2 * side});

  set_ = true;
}


void Renderer::drawAxes(sf::RenderWindow& window, sf::View const& ui_view) const
{
  check_set();
  window.setView(ui_view); 

  sf::Vertex x_axis[] = {{{axis_offset_, y0_}, sf::Color::Black},
                         {{axis_offset_ + axis_length_, y0_}, sf::Color::Black}}; 

  sf::Vertex y_axis[] = {{{axis_offset_, y0_}, sf::Color::Black}, {{axis_offset_, axis_offset_}, sf::Color::Black}}; 

  window.draw(x_axis, 2, sf::Lines); 
  window.draw(y_axis, 2, sf::Lines); 
}

void Renderer::drawTicks(sf::RenderWindow& window, sf::View const& ui_view)
{
  check_set();
  window.setView(ui_view); 

  auto const tick_color = sf::Color::Black;
  auto const grid_color = sf::Color(200, 200, 200, 100); 

  int precision            = std::max(0, -static_cast<int>(std::floor(std::log10(tick_step_)))); 
  std::string const format = "{:." + std::to_string(precision) + "f}";                           

  for (double pos = 0.; pos <= world_max_; pos += tick_step_) {
    float const px = axis_offset_ + static_cast<float>(pos * pixels_per_unit_); 
    float const py = y0_ - static_cast<float>(pos * pixels_per_unit_);          
    if (px > axis_offset_ + axis_length_) {
      break;
    } 
    if (py < axis_offset_) {
      break;
    } 

    sf::Vertex const tick_x[] = {{{px, y0_ - 5.f}, tick_color}, {{px, y0_ + 5.f}, tick_color}};
    window.draw(tick_x, 2, sf::Lines); 
    sf::Vertex const tick_y[] = {{{axis_offset_ - 5.f, py}, tick_color}, {{axis_offset_ + 5.f, py}, tick_color}};
    window.draw(tick_y, 2, sf::Lines); 

    std::string const label = std::vformat(format, std::make_format_args(pos)); 
    label_.setString(label);
    sf::FloatRect const bounds = label_.getLocalBounds();
    label_.setPosition(px - bounds.width / 2.f, y0_ + 8.f);
    window.draw(label_);
    label_.setPosition(axis_offset_ - bounds.width - 10.f, py - bounds.height);
    window.draw(label_);

    sf::Vertex const grid_x[] = {{{px, y0_}, grid_color}, {{px, axis_offset_}, grid_color}}; 
    window.draw(grid_x, 2, sf::Lines);                                                       
    sf::Vertex const grid_y[] = {{{axis_offset_, py}, grid_color},
                                 {{axis_offset_ + axis_length_, py}, grid_color}}; 
    window.draw(grid_y, 2, sf::Lines);                                             
  }
}

void Renderer::drawTrajectory(sf::RenderWindow& window, Simulation const& simulation, std::size_t current_step, sf::View const& world_view)
{
  check_set();
  window.setView(world_view); 
  update_trajectory(simulation, current_step);

  window.draw(trajectory_);
}

void Renderer::drawEqPoints(sf::RenderWindow& window, sf::View const& ui_view, sf::View const& world_view) const
{
  check_set();
  window.setView(world_view); 

  float const radius_world = eq_point_radius_ / static_cast<float>(pixels_per_unit_); 
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

void Renderer::drawTitles(sf::RenderWindow& window, sf::View const& ui_view) const
{
  check_set();
  window.setView(ui_view); 

  sf::Text title;
  title.setFont(font_);
  title.setString("LOTKA-VOLTERRA SIMULATION RENDERING");
  title.setCharacterSize(static_cast<unsigned int>(axis_offset_ / 3.f));
  title.setFillColor(sf::Color::Black);

  sf::FloatRect const bounds = title.getLocalBounds();
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
  x_title.setPosition(axis_offset_ + axis_length_ / 2.f, ui_view.getSize().y - axis_offset_ / 2.f);

  window.draw(x_title);


  sf::Text y_title;
  y_title.setFont(font_);
  y_title.setString("predator");
  y_title.setCharacterSize(static_cast<unsigned int>(axis_offset_ / 4.f));
  y_title.setFillColor(sf::Color::Black);

  sf::FloatRect y_bounds = y_title.getLocalBounds();
  y_title.setOrigin(y_bounds.width / 2.f, y_bounds.height / 2.f);
  y_title.setRotation(-90.f); 
  y_title.setPosition(axis_offset_ / 3.f, y0_ - axis_length_ / 2.f);

  window.draw(y_title);
}

void Renderer::draw(sf::RenderWindow& window, Simulation const& simulation, std::size_t current_step)
{
  validate_window(window);
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