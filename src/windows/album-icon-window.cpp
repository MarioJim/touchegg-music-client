#include "album-icon-window.h"

#include <cairo.h>
#include <gdk/gdk.h>

AlbumIconWindow::AlbumIconWindow(const WindowSystem& window_system,
                                 const WindowsConfig& windows_config)
    : cairo_surface(window_system.createCairoSurface()),
      monitor(window_system.getDesktopWorkarea()),
      windows_config(windows_config) {}

void AlbumIconWindow::render(GdkPixbuf* pixbuf) {
  if (pixbuf == last_pixbuf) {
    return;
  }
  last_pixbuf = pixbuf;

  if (pixbuf == nullptr) {
    cairo_surface->flush();
    return;
  }

  double background_height = windows_config.backgroundHeight();
  double background_x = windows_config.indicatorBackgroundX(monitor) +
                        windows_config.indicatorBackgroundWidth() +
                        windows_config.kMarginBetweenWindows;
  double background_y = windows_config.backgroundY(monitor);
  double album_icon_area_size = background_height;
  double album_icon_size =
      album_icon_area_size - 2 * windows_config.kMarginAlbumIcon;

  int album_icon_size_int = static_cast<int>(album_icon_size);
  int album_icon_x =
      static_cast<int>(background_x + windows_config.kMarginAlbumIcon);
  int album_icon_y =
      static_cast<int>(background_y + windows_config.kMarginAlbumIcon);
  GdkPixbuf* scaled_album_icon = gdk_pixbuf_scale_simple(
      pixbuf, album_icon_size_int, album_icon_size_int, GDK_INTERP_BILINEAR);
  cairo_t* album_icon_ctx = cairo_surface->getContext();
  gdk_cairo_set_source_pixbuf(album_icon_ctx, scaled_album_icon, album_icon_x,
                              album_icon_y);
  cairo_paint(album_icon_ctx);
  cairo_fill(album_icon_ctx);

  cairo_surface->flush();
}
