#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_H

#include <iostream>
#include <string>
#include <utility>

class Metadata {
 public:
  explicit Metadata(std::string song, std::string album, std::string artist)
      : song(std::move(song)),
        album(std::move(album)),
        artist(std::move(artist)) {}

  friend std::ostream &operator<<(std::ostream &out, const Metadata &metadata) {
    out << "Song: " << metadata.song << "\nAlbum: " << metadata.album
        << "\nArtist: " << metadata.artist << std::endl;
    return out;
  }

 private:
  std::string song;
  std::string album;
  std::string artist;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_H
