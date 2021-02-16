#include "window.h"

#include <algorithm>
#include <iostream>

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

void Window::renderMusicWindow(cairo_t* ctx,
                               std::unique_ptr<Metadata> metadata) {
  auto [song_str, album_str, artist_str] = metadata->toTuple();

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

  // Write the song name
  cairo_set_font_size(ctx, 36);
  double song_x = background_x + kMusicBackgroundHorizPadding;
  double song_y = background_y + kSongStringY;
  cairo_move_to(ctx, song_x, song_y);
  cairo_show_text(ctx, song_str.c_str());

  // Write the artist's name
  cairo_set_font_size(ctx, 20);
  double artist_x = background_x + kMusicBackgroundHorizPadding;
  double artist_y = background_y + kArtistStringY;
  cairo_move_to(ctx, artist_x, artist_y);
  cairo_show_text(ctx, artist_str.c_str());
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