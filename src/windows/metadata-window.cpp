#include "windows/metadata-window.h"

#include <algorithm>

MetadataWindow::MetadataWindow(const WindowSystem& window_system,
                               const WindowsConfig& windows_config)
    : cairo_surface(window_system.createCairoSurface()),
      monitor(window_system.getDesktopWorkarea()),
      windows_config(windows_config) {}

void MetadataWindow::render(double volume_percentage,
                            const std::shared_ptr<const Metadata>& metadata) {
  cairo_t* ctx = cairo_surface->getContext();

  // Clear the background
  cairo_set_source_rgba(ctx, 0, 0, 0, 0);
  cairo_set_operator(ctx, CAIRO_OPERATOR_SOURCE);
  cairo_paint(ctx);

  renderVolumeWindow(ctx, volume_percentage);
  if (metadata != nullptr) {
    renderMusicWindow(ctx, metadata);
    cairo_surface->flush();
  }
}

void MetadataWindow::renderVolumeWindow(cairo_t* ctx,
                                        double volume_percentage) {
  double background_x = windows_config.indicatorBackgroundX(monitor);
  double background_y = windows_config.backgroundY(monitor);
  double background_width = windows_config.indicatorBackgroundWidth();
  double background_height = windows_config.backgroundHeight();

  double indicator_x =
      background_x + windows_config.kIndicatorBackgroundHorizMargin;

  // Draw the window background
  cairo_rectangle(ctx, background_x, background_y, background_width,
                  background_height);
  cairo_set_source_rgba(ctx, 0, 0, 0, 0.9);
  cairo_fill(ctx);

  // Draw the volume indicator background
  cairo_rectangle(ctx, indicator_x, windows_config.indicatorY(monitor, 100),
                  windows_config.kIndicatorWidth,
                  windows_config.indicatorHeight(100));
  cairo_set_source_rgba(ctx, 100 / 255.0, 100 / 255.0, 100 / 255.0, 1);
  cairo_fill(ctx);

  // Draw the volume indicator
  cairo_rectangle(ctx, indicator_x,
                  windows_config.indicatorY(monitor, volume_percentage),
                  windows_config.kIndicatorWidth,
                  windows_config.indicatorHeight(volume_percentage));
  cairo_set_source_rgba(ctx, 62 / 255.0, 159 / 255.0, 237 / 255.0, 1);
  cairo_fill(ctx);

  // Write the volume text
  std::string volume_string =
      std::to_string(static_cast<int>(volume_percentage));
  cairo_text_extents_t extents;
  cairo_set_font_size(ctx, 14);
  cairo_text_extents(ctx, volume_string.c_str(), &extents);
  double txt_x = background_x + (background_width - extents.width) / 2;
  double txt_y = background_y + background_height - 18;
  cairo_set_source_rgba(ctx, 1, 1, 1, 1);
  cairo_move_to(ctx, txt_x, txt_y);
  cairo_show_text(ctx, volume_string.c_str());
}

void MetadataWindow::renderMusicWindow(
    cairo_t* ctx, const std::shared_ptr<const Metadata>& metadata) {
  double background_height = windows_config.backgroundHeight();
  double background_x = windows_config.indicatorBackgroundX(monitor) +
                        windows_config.indicatorBackgroundWidth() +
                        windows_config.kMarginBetweenWindows;
  double background_y = windows_config.backgroundY(monitor);
  double right_margin = metadata->album_icon == nullptr
                            ? windows_config.kMusicBackgroundHorizPadding
                            : background_height;

  // Draw the window background
  cairo_rectangle(ctx, background_x, background_y,
                  windows_config.kMusicBackgroundWidth, background_height);
  cairo_set_source_rgba(ctx, 0, 0, 0, 0.9);
  cairo_fill(ctx);

  // Set text color
  cairo_set_source_rgba(ctx, 1, 1, 1, 1);

  double max_text_width = windows_config.kMusicBackgroundWidth -
                          windows_config.kMusicBackgroundHorizPadding -
                          right_margin;
  // Write the song name
  cairo_set_font_size(ctx, 32);
  std::string song = trimText(ctx, metadata->song, max_text_width);
  double song_x = background_x + right_margin;
  double song_y = background_y + windows_config.kSongStringY;
  cairo_move_to(ctx, song_x, song_y);
  cairo_show_text(ctx, song.c_str());

  // Write the artist's name
  cairo_set_font_size(ctx, 20);
  std::string artist = trimText(ctx, metadata->artist, max_text_width);
  double artist_x = background_x + right_margin;
  double artist_y = background_y + windows_config.kArtistStringY;
  cairo_move_to(ctx, artist_x, artist_y);
  cairo_show_text(ctx, artist.c_str());

  // Display the playback status icon
  renderPlaybackStatusIcon(ctx, metadata->playback_status, background_x,
                           background_y);
}

void MetadataWindow::renderPlaybackStatusIcon(cairo_t* ctx,
                                              PlaybackStatus status,
                                              double music_window_x,
                                              double music_window_y) const {
  double startX = music_window_x + windows_config.kMusicBackgroundWidth -
                  windows_config.kMusicBackgroundHorizPadding -
                  windows_config.kPlaybackIconSize;
  double startY = music_window_y + windows_config.kMusicBackgroundHorizPadding;
  double icon_size = windows_config.kPlaybackIconSize;
  double pause_line_width = icon_size / 3.0;

  cairo_set_source_rgba(ctx, 1, 1, 1, 1);

  switch (status) {
    case PlaybackStatus::PLAYING:
      cairo_move_to(ctx, startX, startY);
      cairo_rel_line_to(ctx, 0.0, icon_size);
      cairo_rel_line_to(ctx, icon_size * 0.9, -icon_size / 2);
      cairo_close_path(ctx);
      cairo_fill(ctx);
      break;
    case PlaybackStatus::PAUSED:
      cairo_rectangle(ctx, startX, startY, pause_line_width, icon_size);
      cairo_fill(ctx);
      cairo_rectangle(ctx, startX + 2 * pause_line_width, startY,
                      pause_line_width, icon_size);
      cairo_fill(ctx);
      break;
    case PlaybackStatus::STOPPED:
      cairo_rectangle(ctx, startX, startY, icon_size, icon_size);
      cairo_fill(ctx);
      break;
    default:
      break;
  }
}

std::string MetadataWindow::trimText(cairo_t* ctx, std::string text,
                                     double max_width) {
  cairo_text_extents_t extents;

  cairo_text_extents(ctx, text.c_str(), &extents);
  double text_width = extents.width;

  if (text_width < max_width) {
    return text;
  }

  cairo_text_extents(ctx, kEllipsis, &extents);
  double ellipsis_width = extents.width;
  max_width -= ellipsis_width;

  int chars_to_keep = static_cast<int>(max_width * text.size() / text_width);
  std::string cropped_text = text.substr(0, chars_to_keep);
  cairo_text_extents(ctx, cropped_text.c_str(), &extents);
  double cropped_text_width = extents.width;

  while (cropped_text_width > max_width) {
    cropped_text.pop_back();
    cairo_text_extents(ctx, cropped_text.c_str(), &extents);
    cropped_text_width = extents.width;
  }

  return cropped_text.append(kEllipsis);
}
