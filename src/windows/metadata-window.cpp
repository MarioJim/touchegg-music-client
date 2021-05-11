#include "windows/metadata-window.h"

#include <cairomm/cairomm.h>

#include <algorithm>
#include <utility>

MetadataWindow::MetadataWindow(std::unique_ptr<CairoSurface> cairo_surface,
                               const Rectangle& monitor,
                               const WindowsConfig& windows_config)
    : cairo_surface(std::move(cairo_surface)),
      monitor(monitor),
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

  Cairo::Context ctx(cairo_surface->getContext(), false);

  // Clear the background
  ctx.set_source_rgba(0, 0, 0, 0);
  ctx.set_operator(Cairo::Operator::OPERATOR_SOURCE);
  ctx.paint();

  double background_height = windows_config.backgroundHeight();
  double background_x = windows_config.indicatorBackgroundX(monitor) +
                        windows_config.indicatorBackgroundWidth() +
                        windows_config.kMarginBetweenWindows;
  double background_y = windows_config.backgroundY(monitor);
  double right_margin = metadata->hasAlbumIcon()
                            ? background_height
                            : windows_config.kMusicBackgroundHorizPadding;

  // Draw the window background
  ctx.rectangle(background_x, background_y,
                windows_config.kMusicBackgroundWidth, background_height);
  ctx.set_source_rgba(0, 0, 0, 0.9);
  ctx.fill();

  // Set text color
  ctx.set_source_rgba(1, 1, 1, 1);

  double max_text_width = windows_config.kMusicBackgroundWidth -
                          windows_config.kMusicBackgroundHorizPadding -
                          right_margin;
  // Write the song name
  ctx.set_font_size(32);
  double song_x = background_x + right_margin;
  double song_y = background_y + windows_config.kSongStringY;
  ctx.move_to(song_x, song_y);
  std::string song = trimText(ctx, metadata->getSong(), max_text_width);
  ctx.show_text(song);

  // Write the artist's name
  ctx.set_font_size(20);
  double artist_x = background_x + right_margin;
  double artist_y = background_y + windows_config.kArtistStringY;
  ctx.move_to(artist_x, artist_y);
  std::string artist = trimText(ctx, metadata->getArtist(), max_text_width);
  ctx.show_text(artist);

  // Display the playback status icon
  renderPlaybackStatusIcon(&ctx, metadata->getPlaybackStatus(), background_x,
                           background_y);

  cairo_surface->flush();
}

void MetadataWindow::renderPlaybackStatusIcon(Cairo::Context* ctx,
                                              PlaybackStatus status,
                                              double music_window_x,
                                              double music_window_y) const {
  double startX = music_window_x + windows_config.kMusicBackgroundWidth -
                  windows_config.kMusicBackgroundHorizPadding -
                  windows_config.kPlaybackIconSize;
  double startY = music_window_y + windows_config.kMusicBackgroundHorizPadding;
  double icon_size = windows_config.kPlaybackIconSize;
  double pause_line_width = icon_size / 3.0;

  ctx->set_source_rgba(1, 1, 1, 1);

  switch (status) {
    case PlaybackStatus::PLAYING:
      ctx->move_to(startX, startY);
      ctx->rel_line_to(0.0, icon_size);
      ctx->rel_line_to(icon_size * 0.9, -icon_size / 2);
      ctx->close_path();
      ctx->fill();
      break;
    case PlaybackStatus::PAUSED:
      ctx->rectangle(startX, startY, pause_line_width, icon_size);
      ctx->fill();
      ctx->rectangle(startX + 2 * pause_line_width, startY, pause_line_width,
                     icon_size);
      ctx->fill();
      break;
    case PlaybackStatus::STOPPED:
      ctx->rectangle(startX, startY, icon_size, icon_size);
      ctx->fill();
      break;
    default:
      break;
  }
}

std::string MetadataWindow::trimText(const Cairo::Context& ctx,
                                     const std::string& text,
                                     double max_width) {
  Cairo::TextExtents extents;

  ctx.get_text_extents(text, extents);
  double text_width = extents.width;

  if (text_width < max_width) {
    return text;
  }

  ctx.get_text_extents(kEllipsis, extents);
  double ellipsis_width = extents.width;
  max_width -= ellipsis_width;

  auto text_length = static_cast<double>(text.size());
  int chars_to_keep = static_cast<int>(max_width * text_length / text_width);
  std::string new_text = text.substr(0, chars_to_keep);
  ctx.get_text_extents(new_text, extents);
  double cropped_text_width = extents.width;

  while (cropped_text_width > max_width) {
    new_text.pop_back();
    ctx.get_text_extents(new_text, extents);
    cropped_text_width = extents.width;
  }

  new_text.append(kEllipsis);
  return new_text;
}
