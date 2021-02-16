#ifndef TOUCHEGG_MUSIC_CLIENT_WINDOW_H
#define TOUCHEGG_MUSIC_CLIENT_WINDOW_H

#include <cairo.h>

#include <tuple>

#include "metadata/metadata.h"
#include "utils/rectangle.h"
#include "window-system/cairo-surface.h"
#include "window-system/window-system.h"

class Window {
 public:
  explicit Window(const WindowSystem& window_system);
  virtual ~Window() = default;

  void render(double volume_percentage, std::unique_ptr<Metadata> metadata);

 private:
  void renderMusicWindow(cairo_t* ctx, std::unique_ptr<Metadata> metadata);
  void renderVolumeWindow(cairo_t* ctx, double volume_percentage);

  [[nodiscard]] inline double calculateIndicatorY(double percentage) const;
  static inline double calculateIndicatorHeight(double percentage);
  [[nodiscard]] inline double calculateIndicatorBackgroundX() const;
  [[nodiscard]] inline double calculateIndicatorBackgroundY() const;
  [[nodiscard]] inline double calculateIndicatorBackgroundWidth() const;
  [[nodiscard]] inline double calculateIndicatorBackgroundHeight() const;

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
};

#endif  // TOUCHEGG_MUSIC_CLIENT_WINDOW_H
