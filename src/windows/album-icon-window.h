#ifndef TOUCHEGG_MUSIC_CLIENT_ALBUM_ICON_WINDOW_H
#define TOUCHEGG_MUSIC_CLIENT_ALBUM_ICON_WINDOW_H

#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <memory>

#include "utils/rectangle.h"
#include "window-system/cairo-surface.h"
#include "window-system/window-system.h"

class AlbumIconWindow {
 public:
  explicit AlbumIconWindow(const WindowSystem& window_system);

  void render(GdkPixbuf* pixbuf);

 private:
  std::unique_ptr<CairoSurface> cairo_surface;
  Rectangle monitor;
  GdkPixbuf* last_pixbuf;

  const double kMarginAlbumIcon = 20;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_ALBUM_ICON_WINDOW_H
