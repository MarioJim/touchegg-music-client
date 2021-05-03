#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_H

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-object.h>

#include <string>
#include <utility>

#include "metadata/playback-status.h"

struct Metadata {
 public:
  Metadata(std::string song, std::string album, std::string artist,
           PlaybackStatus status, GdkPixbuf* album_icon)
      : song(std::move(song)),
        album(std::move(album)),
        artist(std::move(artist)),
        playback_status(status),
        album_icon(album_icon) {}

  ~Metadata() {
    if (album_icon != nullptr) {
      g_object_unref(album_icon);
    }
  }

  Metadata(const Metadata& other) = delete;
  Metadata& operator=(const Metadata& other) = delete;
  Metadata(Metadata&& other) noexcept = delete;
  Metadata& operator=(Metadata&& other) noexcept = delete;

  [[nodiscard]] const std::string& getSong() const { return song; }
  [[nodiscard]] const std::string& getArtist() const { return artist; }
  [[nodiscard]] PlaybackStatus getPlaybackStatus() const {
    return playback_status;
  }
  [[nodiscard]] GdkPixbuf* getAlbumIcon() const { return album_icon; }
  [[nodiscard]] bool hasAlbumIcon() const { return album_icon != nullptr; }

 private:
  const std::string song, album, artist;
  const PlaybackStatus playback_status;
  GdkPixbuf* album_icon;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_H
