#include "window.h"

#include <algorithm>
#include <string>
#include <utility>

Window::Window(const WindowSystem& window_system)
    : cairo_surface(window_system.createCairoSurface()),
      monitor(window_system.getDesktopWorkarea()) {}

void Window::render(double volume_percentage,
                    std::unique_ptr<Metadata> metadata) {
  cairo_t* ctx = cairo_surface->getContext();

  // Clear the background
  cairo_set_source_rgba(ctx, 0, 0, 0, 0);
  cairo_set_operator(ctx, CAIRO_OPERATOR_SOURCE);
  cairo_paint(ctx);

  renderVolumeWindow(ctx, volume_percentage);
  if (metadata != nullptr) {
    renderMusicWindow(ctx, std::move(metadata));
  }

  cairo_surface->flush();
}

void Window::renderVolumeWindow(cairo_t* ctx, double volume_percentage) {
  double background_x = calculateIndicatorBackgroundX();
  double background_y = calculateIndicatorBackgroundY();
  double background_width = calculateIndicatorBackgroundWidth();
  double background_height = calculateIndicatorBackgroundHeight();

  double indicator_x = background_x + kIndicatorBackgroundHorizMargin;

  // Draw the window background
  cairo_rectangle(ctx, background_x, background_y, background_width,
                  background_height);
  cairo_set_source_rgba(ctx, 0, 0, 0, 0.9);
  cairo_fill(ctx);

  // Draw the volume indicator background
  cairo_rectangle(ctx, indicator_x, calculateIndicatorY(100), kIndicatorWidth,
                  calculateIndicatorHeight(100));
  cairo_set_source_rgba(ctx, 100 / 255.0, 100 / 255.0, 100 / 255.0, 1);
  cairo_fill(ctx);

  // Draw the volume indicator
  cairo_rectangle(ctx, indicator_x, calculateIndicatorY(volume_percentage),
                  kIndicatorWidth, calculateIndicatorHeight(volume_percentage));
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

void Window::renderMusicWindow(cairo_t* ctx,
                               std::unique_ptr<Metadata> metadata) {
  double background_height = calculateIndicatorBackgroundHeight();
  double background_x = calculateIndicatorBackgroundX() +
                        calculateIndicatorBackgroundWidth() +
                        kMarginBetweenWindows;
  double background_y = calculateIndicatorBackgroundY();

  // Draw the window background
  cairo_rectangle(ctx, background_x, background_y, kMusicBackgroundWidth,
                  background_height);
  cairo_set_source_rgba(ctx, 0, 0, 0, 0.9);
  cairo_fill(ctx);

  // Set text color
  cairo_set_source_rgba(ctx, 1, 1, 1, 1);

  double max_text_width = kMusicBackgroundWidth - 2 * kMusicBackgroundHorizPadding;
  // Write the song name
  cairo_set_font_size(ctx, 32);
  std::string song = trimText(ctx, metadata->song, max_text_width);
  double song_x = background_x + kMusicBackgroundHorizPadding;
  double song_y = background_y + kSongStringY;
  cairo_move_to(ctx, song_x, song_y);
  cairo_show_text(ctx, song.c_str());

  // Write the artist's name
  cairo_set_font_size(ctx, 20);
  std::string artist = trimText(ctx, metadata->artist, max_text_width);
  double artist_x = background_x + kMusicBackgroundHorizPadding;
  double artist_y = background_y + kArtistStringY;
  cairo_move_to(ctx, artist_x, artist_y);
  cairo_show_text(ctx, artist.c_str());

  // Display the playback status icon
  renderPlaybackStatusIcon(ctx, metadata->playback_status, background_x,
                           background_y);
}

void Window::renderPlaybackStatusIcon(cairo_t* ctx, PlaybackStatus status,
                                      double music_window_x,
                                      double music_window_y) const {
  double startX = music_window_x + kMusicBackgroundWidth -
                  kMusicBackgroundHorizPadding - kPlaybackIconSize;
  double startY = music_window_y + kMusicBackgroundHorizPadding;
  double pause_line_width = kPlaybackIconSize / 3.0;

  cairo_set_source_rgba(ctx, 1, 1, 1, 1);

  switch (status) {
    case PlaybackStatus::PLAYING:
      cairo_move_to(ctx, startX, startY);
      cairo_rel_line_to(ctx, 0.0, kPlaybackIconSize);
      cairo_rel_line_to(ctx, kPlaybackIconSize * 0.9, -kPlaybackIconSize / 2);
      cairo_close_path(ctx);
      cairo_fill(ctx);
      break;
    case PlaybackStatus::PAUSED:
      cairo_rectangle(ctx, startX, startY, pause_line_width, kPlaybackIconSize);
      cairo_fill(ctx);
      cairo_rectangle(ctx, startX + 2 * pause_line_width, startY,
                      pause_line_width, kPlaybackIconSize);
      cairo_fill(ctx);
      break;
    case PlaybackStatus::STOPPED:
      cairo_rectangle(ctx, startX, startY, kPlaybackIconSize,
                      kPlaybackIconSize);
      cairo_fill(ctx);
      break;
    default:
      break;
  }
}

double Window::calculateIndicatorY(double percentage) const {
  return monitor.y + 190 - calculateIndicatorHeight(percentage);
}

double Window::calculateIndicatorHeight(double percentage) {
  return 0.9 * std::clamp(percentage, 0.0, 100.0);
}

double Window::calculateIndicatorBackgroundX() const { return monitor.x + 64; }

double Window::calculateIndicatorBackgroundY() const {
  return calculateIndicatorY(100) - kIndicatorBackgroundTopMargin;
}

double Window::calculateIndicatorBackgroundWidth() const {
  return kIndicatorWidth + 2 * kIndicatorBackgroundHorizMargin;
}

double Window::calculateIndicatorBackgroundHeight() const {
  return calculateIndicatorHeight(100) + kIndicatorBackgroundTopMargin +
         kIndicatorBackgroundBottomMargin;
}

std::string Window::trimText(cairo_t* ctx, std::string text, double max_width) {
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
