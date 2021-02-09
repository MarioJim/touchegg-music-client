#include "volume-window.h"

#include <algorithm>
#include <sstream>

VolumeWindow::VolumeWindow(const WindowSystem& window_system)
    : cairo_surface(window_system.createCairoSurface()),
      monitor(window_system.getDesktopWorkarea()) {}

void VolumeWindow::render(double volume_percentage) {
  cairo_t* ctx = cairo_surface->getContext();

  // Clear the background
  cairo_set_source_rgba(ctx, 0, 0, 0, 0);
  cairo_set_operator(ctx, CAIRO_OPERATOR_SOURCE);
  cairo_paint(ctx);

  double window_alpha = 0.9;

  double ind_width = 20;
  auto calc_ind_height = [](double percentage) {
    return std::clamp(percentage, 0.0, 100.0);
  };
  double ind_x = monitor.x + 70;
  auto calc_ind_y = [m = &monitor, &calc_ind_height](double percentage) {
    return m->y + 200 - calc_ind_height(percentage);
  };

  double bg_horizontal_margin = 26;
  double bg_top_margin = 24;
  double bg_bottom_margin = 40;
  double bg_width = ind_width + 2 * bg_horizontal_margin;
  double bg_height = calc_ind_height(100) + bg_top_margin + bg_bottom_margin;
  double bg_x = ind_x - bg_horizontal_margin;
  double bg_y = calc_ind_y(100) - bg_top_margin;

  // Draw the volume indicator background
  cairo_rectangle(ctx, bg_x, bg_y, bg_width, bg_height);
  cairo_set_source_rgba(ctx, 0, 0, 0, window_alpha);
  cairo_fill(ctx);

  // Draw the volume indicator
  cairo_rectangle(ctx, ind_x, calc_ind_y(volume_percentage), ind_width,
                  calc_ind_height(volume_percentage));
  cairo_set_source_rgba(ctx, 62 / 255.0, 159 / 255.0, 237 / 255.0,
                        window_alpha);
  cairo_fill(ctx);

  // Write the volume text
  std::string volume_string{
      std::to_string(static_cast<int>(volume_percentage))};
  cairo_text_extents_t extents;
  cairo_text_extents(ctx, volume_string.c_str(), &extents);
  double txt_x = bg_x + (bg_width - extents.width) / 2;
  double txt_y = bg_y + bg_height - 20;

  cairo_set_source_rgba(ctx, 1, 1, 1, window_alpha);
  cairo_set_font_size(ctx, 16);
  cairo_move_to(ctx, txt_x, txt_y);
  cairo_show_text(ctx, volume_string.c_str());

  cairo_surface->flush();
}
