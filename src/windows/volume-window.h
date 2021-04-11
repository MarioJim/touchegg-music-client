#ifndef TOUCHEGG_MUSIC_CLIENT_VOLUME_WINDOW_H
#define TOUCHEGG_MUSIC_CLIENT_VOLUME_WINDOW_H

#include "utils/rectangle.h"
#include "window-system/cairo-surface.h"
#include "window-system/window-system.h"
#include "windows/windows-config.h"

class VolumeWindow {
 public:
  explicit VolumeWindow(const WindowSystem& window_system,
                        const WindowsConfig& windows_config);

  void render(double volume_percentage);

 private:
  std::unique_ptr<CairoSurface> cairo_surface;
  const Rectangle monitor;
  const WindowsConfig& windows_config;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_VOLUME_WINDOW_H
