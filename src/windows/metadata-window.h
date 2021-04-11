#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H

#include <cairo.h>

#include <memory>
#include <string>

#include "metadata/metadata.h"
#include "utils/rectangle.h"
#include "window-system/cairo-surface.h"
#include "window-system/window-system.h"
#include "windows/windows-config.h"

class MetadataWindow {
 public:
  explicit MetadataWindow(const WindowSystem& window_system,
                          const WindowsConfig& windows_config);

  void render(const std::shared_ptr<const Metadata>& metadata);

 private:
  void renderMusicWindow(cairo_t* ctx,
                         const std::shared_ptr<const Metadata>& metadata);
  void renderPlaybackStatusIcon(cairo_t* ctx, PlaybackStatus status,
                                double music_window_x,
                                double music_window_y) const;

  static std::string trimText(cairo_t* ctx, std::string text, double max_width);

  std::unique_ptr<CairoSurface> cairo_surface;
  const Rectangle monitor;
  const WindowsConfig& windows_config;

  static constexpr const char* kEllipsis = "...";
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H
