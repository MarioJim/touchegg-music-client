#include "volume-window.h"

VolumeWindow::VolumeWindow(const WindowSystem& window_system)
    : cairo_surface(window_system.createCairoSurface()),
      monitor(window_system.getDesktopWorkarea()) {}

void VolumeWindow::render(double volume_percentage) {
  cairo_t* ctx = cairo_surface->getContext();

  // Clear the background
  cairo_set_source_rgba(ctx, 0, 0, 0, 0);
  cairo_set_operator(ctx, CAIRO_OPERATOR_SOURCE);
  cairo_paint(ctx);

  // Draw the volume indicator
  cairo_rectangle(ctx, monitor.x + 50, monitor.y + 200 - volume_percentage, 20,
                  volume_percentage);
  cairo_set_source_rgba(ctx, 62 / 255.0, 159 / 255.0, 237 / 255.0, 1);
  cairo_fill(ctx);

  cairo_surface->flush();
}
