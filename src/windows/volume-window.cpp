#include "windows/volume-window.h"

#include <cairo.h>

#include <string>

VolumeWindow::VolumeWindow(std::unique_ptr<CairoSurface> cairo_surface,
                           const Rectangle& monitor,
                           const WindowsConfig& windows_config)
    : cairo_surface(std::move(cairo_surface)),
      monitor(monitor),
      windows_config(windows_config) {}

void VolumeWindow::render(double volume_percentage) {
  cairo_t* ctx = cairo_surface->getContext();

  // Clear the background
  cairo_set_source_rgba(ctx, 0, 0, 0, 0);
  cairo_set_operator(ctx, CAIRO_OPERATOR_SOURCE);
  cairo_paint(ctx);

  double background_x = windows_config.indicatorBackgroundX(monitor);
  double background_y = windows_config.backgroundY(monitor);
  double background_width = windows_config.indicatorBackgroundWidth();
  double background_height = windows_config.backgroundHeight();

  double indicator_x =
      background_x + windows_config.kIndicatorBackgroundHorizMargin;

  // Draw the window background
  cairo_rectangle(ctx, background_x, background_y, background_width,
                  background_height);
  cairo_set_source_rgba(ctx, 0, 0, 0, 0.9);
  cairo_fill(ctx);

  // Draw the volume indicator background
  cairo_rectangle(ctx, indicator_x, windows_config.indicatorY(monitor, 100),
                  windows_config.kIndicatorWidth,
                  windows_config.indicatorHeight(100));
  cairo_set_source_rgba(ctx, 100 / 255.0, 100 / 255.0, 100 / 255.0, 1);
  cairo_fill(ctx);

  // Draw the volume indicator
  cairo_rectangle(ctx, indicator_x,
                  windows_config.indicatorY(monitor, volume_percentage),
                  windows_config.kIndicatorWidth,
                  windows_config.indicatorHeight(volume_percentage));
  cairo_set_source_rgba(ctx, 62 / 255.0, 159 / 255.0, 237 / 255.0, 1);
  cairo_fill(ctx);

  // Write the volume text
  std::string volume_string =
      std::to_string(static_cast<int>(volume_percentage));
  cairo_text_extents_t extents;
  cairo_set_font_size(ctx, 14);
  cairo_text_extents(ctx, volume_string.c_str(), &extents);
  double txt_x = background_x + (background_width - extents.width) / 2;
  double txt_y = background_y + background_height - 18;
  cairo_set_source_rgba(ctx, 1, 1, 1, 1);
  cairo_move_to(ctx, txt_x, txt_y);
  cairo_show_text(ctx, volume_string.c_str());

  cairo_surface->flush();
}
