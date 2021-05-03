#ifndef TOUCHEGG_MUSIC_CLIENT_ALBUM_ICON_WINDOW_H
#define TOUCHEGG_MUSIC_CLIENT_ALBUM_ICON_WINDOW_H

#include <gdk-pixbuf/gdk-pixbuf.h>

#include <memory>

#include "utils/rectangle.h"
#include "window-system/cairo-surface.h"
#include "windows/windows-config.h"

class AlbumIconWindow {
 public:
  explicit AlbumIconWindow(std::unique_ptr<CairoSurface> cairo_surface,
                           const Rectangle& monitor,
                           const WindowsConfig& windows_config);

  void render(GdkPixbuf* pixbuf);

 private:
  std::unique_ptr<CairoSurface> cairo_surface;
  const Rectangle monitor;
  const WindowsConfig& windows_config;

  GdkPixbuf* last_pixbuf{nullptr};
};

#endif  // TOUCHEGG_MUSIC_CLIENT_ALBUM_ICON_WINDOW_H
