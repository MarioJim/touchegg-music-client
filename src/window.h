#ifndef TOUCHEGG_MUSIC_CLIENT_WINDOW_H
#define TOUCHEGG_MUSIC_CLIENT_WINDOW_H

#include <cairo.h>

#include "metadata/metadata.h"
#include "utils/rectangle.h"
#include "window-system/cairo-surface.h"
#include "window-system/window-system.h"

class Window {
 public:
  explicit Window(const WindowSystem &window_system);
  virtual ~Window() = default;

  void render(double volume_percentage, std::unique_ptr<Metadata> metadata);

 private:
  std::unique_ptr<CairoSurface> cairo_surface;
  Rectangle monitor;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_WINDOW_H
