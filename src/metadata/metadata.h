#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_H

#include <iostream>
#include <string>
#include <tuple>
#include <utility>

#include "metadata/playback-status.h"

struct Metadata {
  const std::string song, album, artist;
  const PlaybackStatus playback_status;

  Metadata(std::string song, std::string album, std::string artist,
           PlaybackStatus status)
      : song(std::move(song)),
        album(std::move(album)),
        artist(std::move(artist)),
        playback_status(status) {}
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_H
