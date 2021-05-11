#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H

#include <cairomm/cairomm.h>

#include <memory>
#include <string>

#include "metadata/metadata.h"
#include "utils/rectangle.h"
#include "window-system/cairo-surface.h"
#include "windows/windows-config.h"

class MetadataWindow {
 public:
  explicit MetadataWindow(std::unique_ptr<CairoSurface> cairo_surface,
                          const Rectangle& monitor,
                          const WindowsConfig& windows_config);

  void render(const std::shared_ptr<const Metadata>& metadata);

 private:
  inline void renderPlaybackStatusIcon(Cairo::Context* ctx,
                                       PlaybackStatus status,
                                       double music_window_x,
                                       double music_window_y) const;
  static inline std::string trimText(const Cairo::Context& ctx,
                                     const std::string& text, double max_width);

  std::unique_ptr<CairoSurface> cairo_surface;
  const Rectangle monitor;
  const WindowsConfig& windows_config;

  size_t last_metadata_hash{};

  static constexpr const char* kEllipsis = "...";
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_WINDOW_H
