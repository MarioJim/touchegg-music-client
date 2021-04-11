#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H

#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <memory>
#include <string>
#include <tuple>

#include "metadata/metadata.h"
#include "utils/rectangle.h"
#include "window-system/cairo-surface.h"
#include "window-system/window-system.h"

class MetadataWindow {
 public:
  explicit MetadataWindow(const WindowSystem& window_system);

  void render(double volume_percentage,
              std::shared_ptr<const Metadata> metadata);

 private:
  void renderVolumeWindow(cairo_t* ctx, double volume_percentage);
  void renderMusicWindow(cairo_t* ctx,
                         std::shared_ptr<const Metadata> metadata);
  void renderMusicWindowWithAlbum(cairo_t* ctx,
                                  std::shared_ptr<const Metadata> metadata);
  void renderPlaybackStatusIcon(cairo_t* ctx, PlaybackStatus status,
                                double music_window_x,
                                double music_window_y) const;

  [[nodiscard]] inline double calculateIndicatorY(double percentage) const;
  static inline double calculateIndicatorHeight(double percentage);
  [[nodiscard]] inline double calculateIndicatorBackgroundX() const;
  [[nodiscard]] inline double calculateBackgroundY() const;
  [[nodiscard]] inline double calculateIndicatorBackgroundWidth() const;
  [[nodiscard]] inline double calculateBackgroundHeight() const;
  static std::string trimText(cairo_t* ctx, std::string text, double max_width);

  const WindowSystem& window_system;
  std::unique_ptr<CairoSurface> cairo_surface;
  Rectangle monitor;

  const double kIndicatorWidth = 16;
  const double kIndicatorBackgroundHorizMargin = 26;
  const double kIndicatorBackgroundTopMargin = 22;
  const double kIndicatorBackgroundBottomMargin = 40;

  const double kMarginBetweenWindows = 6;

  const double kMusicBackgroundWidth = 500;
  const double kMusicBackgroundHorizPadding = 30;
  const double kSongStringY = 92;
  const double kArtistStringY = kSongStringY + 30;
  const double kPlaybackIconSize = 20;

  static constexpr const char* kEllipsis = "...";
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H
