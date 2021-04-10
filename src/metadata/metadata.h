#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_H

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-object.h>

#include <string>
#include <utility>

#include "metadata/playback-status.h"

struct Metadata {
 public:
  std::string song, album, artist;
  PlaybackStatus playback_status;
  GdkPixbuf* album_icon;

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
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_H
