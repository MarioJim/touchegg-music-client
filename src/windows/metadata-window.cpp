#include "windows/metadata-window.h"

#include <algorithm>

MetadataWindow::MetadataWindow(const WindowSystem& window_system,
                               const WindowsConfig& windows_config)
    : cairo_surface(window_system.createCairoSurface()),
      monitor(window_system.getDesktopWorkarea()),
      windows_config(windows_config) {}

void MetadataWindow::render(const std::shared_ptr<const Metadata>& metadata) {
  size_t metadata_hash = std::hash<std::shared_ptr<const Metadata>>()(metadata);
  if (metadata_hash == last_metadata_hash) {
    return;
  }
  last_metadata_hash = metadata_hash;

  if (metadata == nullptr) {
    return;
  }

  cairo_t* ctx = cairo_surface->getContext();

  // Clear the background
  cairo_set_source_rgba(ctx, 0, 0, 0, 0);
  cairo_set_operator(ctx, CAIRO_OPERATOR_SOURCE);
  cairo_paint(ctx);

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
  double song_x = background_x + right_margin;
  double song_y = background_y + windows_config.kSongStringY;
  cairo_move_to(ctx, song_x, song_y);
  std::unique_ptr<char[]> song = trimText(ctx, metadata->song, max_text_width);
  if (song) {
    cairo_show_text(ctx, song.get());
  } else {
    cairo_show_text(ctx, metadata->song.c_str());
  }

  // Write the artist's name
  cairo_set_font_size(ctx, 20);
  double artist_x = background_x + right_margin;
  double artist_y = background_y + windows_config.kArtistStringY;
  cairo_move_to(ctx, artist_x, artist_y);
  std::unique_ptr<char[]> artist =
      trimText(ctx, metadata->artist, max_text_width);
  if (artist) {
    cairo_show_text(ctx, artist.get());
  } else {
    cairo_show_text(ctx, metadata->artist.c_str());
  }

  // Display the playback status icon
  renderPlaybackStatusIcon(ctx, metadata->playback_status, background_x,
                           background_y);

  cairo_surface->flush();
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

std::unique_ptr<char[]> MetadataWindow::trimText(cairo_t* ctx,
                                                 const std::string& text,
                                                 double max_width) {
  cairo_text_extents_t extents;

  cairo_text_extents(ctx, text.c_str(), &extents);
  double text_width = extents.width;

  if (text_width < max_width) {
    return nullptr;
  }

  cairo_text_extents(ctx, kEllipsis, &extents);
  double ellipsis_width = extents.width;
  max_width -= ellipsis_width;

  auto text_length = static_cast<double>(text.size());
  int chars_to_keep = static_cast<int>(max_width * text_length / text_width);
  std::unique_ptr<char[]> new_text =
      std::make_unique<char[]>(chars_to_keep + 4);
  text.copy(new_text.get(), chars_to_keep);
  new_text[chars_to_keep] = '\0';
  cairo_text_extents(ctx, new_text.get(), &extents);
  double cropped_text_width = extents.width;

  while (cropped_text_width > max_width) {
    --chars_to_keep;
    new_text[chars_to_keep] = '\0';
    cairo_text_extents(ctx, new_text.get(), &extents);
    cropped_text_width = extents.width;
  }

  new_text[chars_to_keep + 0] = '.';
  new_text[chars_to_keep + 1] = '.';
  new_text[chars_to_keep + 2] = '.';
  new_text[chars_to_keep + 3] = '\0';
  return new_text;
}
