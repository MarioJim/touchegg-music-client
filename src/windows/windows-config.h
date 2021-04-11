#ifndef TOUCHEGG_MUSIC_CLIENT_WINDOWS_CONFIG_H
#define TOUCHEGG_MUSIC_CLIENT_WINDOWS_CONFIG_H

#include <utils/rectangle.h>

class WindowsConfig {
 public:
  [[nodiscard]] double indicatorY(const Rectangle &monitor,
                                  double percentage) const;
  [[nodiscard]] double indicatorHeight(double percentage) const;
  [[nodiscard]] double indicatorBackgroundX(const Rectangle &monitor) const;
  [[nodiscard]] double backgroundY(const Rectangle &monitor) const;
  [[nodiscard]] double indicatorBackgroundWidth() const;
  [[nodiscard]] double backgroundHeight() const;

  const double kIndicatorWidth = 16;
  const double kIndicatorBackgroundHorizMargin = 26;
  const double kIndicatorBackgroundTopMargin = 22;
  const double kIndicatorBackgroundBottomMargin = 40;

  const double kMarginBetweenWindows = 6;
  const double kMarginAlbumIcon = 20;

  const double kMusicBackgroundWidth = 500;
  const double kMusicBackgroundHorizPadding = 30;
  const double kSongStringY = 92;
  const double kArtistStringY = kSongStringY + 30;
  const double kPlaybackIconSize = 20;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_WINDOWS_CONFIG_H
