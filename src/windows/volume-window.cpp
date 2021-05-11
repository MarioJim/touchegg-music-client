#include "windows/volume-window.h"

#include <cairomm/cairomm.h>

#include <string>

VolumeWindow::VolumeWindow(std::unique_ptr<CairoSurface> cairo_surface,
                           const Rectangle& monitor,
                           const WindowsConfig& windows_config)
    : cairo_surface(std::move(cairo_surface)),
      monitor(monitor),
      windows_config(windows_config) {}

void VolumeWindow::render(double volume_percentage) {
  Cairo::Context ctx(cairo_surface->getContext(), false);

  // Clear the background
  ctx.set_source_rgba(0, 0, 0, 0);
  ctx.set_operator(Cairo::Operator::OPERATOR_SOURCE);
  ctx.paint();

  double background_x = windows_config.indicatorBackgroundX(monitor);
  double background_y = windows_config.backgroundY(monitor);
  double background_width = windows_config.indicatorBackgroundWidth();
  double background_height = windows_config.backgroundHeight();

  double indicator_x =
      background_x + windows_config.kIndicatorBackgroundHorizMargin;

  // Draw the window background
  ctx.rectangle(background_x, background_y, background_width,
                background_height);
  ctx.set_source_rgba(0, 0, 0, 0.9);
  ctx.fill();

  // Draw the volume indicator background
  ctx.rectangle(indicator_x, windows_config.indicatorY(monitor, 100),
                windows_config.kIndicatorWidth,
                windows_config.indicatorHeight(100));
  ctx.set_source_rgba(100 / 255.0, 100 / 255.0, 100 / 255.0, 1);
  ctx.fill();

  // Draw the volume indicator
  ctx.rectangle(indicator_x,
                windows_config.indicatorY(monitor, volume_percentage),
                windows_config.kIndicatorWidth,
                windows_config.indicatorHeight(volume_percentage));
  ctx.set_source_rgba(62 / 255.0, 159 / 255.0, 237 / 255.0, 1);
  ctx.fill();

  // Write the volume text
  std::string volume_string =
      std::to_string(static_cast<int>(volume_percentage));
  Cairo::TextExtents extents;
  ctx.set_font_size(14);
  ctx.get_text_extents(volume_string, extents);
  double txt_x = background_x + (background_width - extents.width) / 2;
  double txt_y = background_y + background_height - 18;
  ctx.set_source_rgba(1, 1, 1, 1);
  ctx.move_to(txt_x, txt_y);
  ctx.show_text(volume_string);

  cairo_surface->flush();
}
