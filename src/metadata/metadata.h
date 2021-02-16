#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_H

#include <iostream>
#include <string>
#include <tuple>
#include <utility>

class Metadata {
 public:
  explicit Metadata(std::string song, std::string album, std::string artist)
      : song(std::move(song)),
        album(std::move(album)),
        artist(std::move(artist)) {}

  std::tuple<std::string, std::string, std::string> toTuple() {
    return std::make_tuple(song, album, artist);
  }

 private:
  std::string song;
  std::string album;
  std::string artist;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_H
