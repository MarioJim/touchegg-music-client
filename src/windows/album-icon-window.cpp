#include "windows/album-icon-window.h"

#include <cairomm/cairomm.h>
#include <gdk/gdk.h>
#include <glib-object.h>

#include <utility>

AlbumIconWindow::AlbumIconWindow(std::unique_ptr<CairoSurface> cairo_surface,
                                 const Rectangle& monitor,
                                 const WindowsConfig& windows_config)
    : cairo_surface(std::move(cairo_surface)),
      monitor(monitor),
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
  double icon_area_size = background_height;
  double icon_size = icon_area_size - 2 * windows_config.kMarginAlbumIcon;

  int icon_size_int = static_cast<int>(icon_size);
  int icon_x = static_cast<int>(background_x + windows_config.kMarginAlbumIcon);
  int icon_y = static_cast<int>(background_y + windows_config.kMarginAlbumIcon);

  GdkPixbuf* scaled_icon = gdk_pixbuf_scale_simple(
      pixbuf, icon_size_int, icon_size_int, GDK_INTERP_BILINEAR);
  Cairo::Context ctx(cairo_surface->getContext(), false);
  gdk_cairo_set_source_pixbuf(ctx.cobj(), scaled_icon, icon_x, icon_y);
  ctx.paint();

  cairo_surface->flush();

  g_object_unref(scaled_icon);
}
