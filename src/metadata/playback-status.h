#ifndef TOUCHEGG_MUSIC_CLIENT_PLAYBACK_STATUS_H
#define TOUCHEGG_MUSIC_CLIENT_PLAYBACK_STATUS_H

#include <string>

enum class PlaybackStatus {
  PLAYING,
  PAUSED,
  STOPPED,
  UNKNOWN,
};

inline PlaybackStatus playbackStatusFromString(const std::string &status) {
  if (status == "Playing") {
    return PlaybackStatus::PLAYING;
  }
  if (status == "Paused") {
    return PlaybackStatus::PAUSED;
  }
  if (status == "Stopped") {
    return PlaybackStatus::STOPPED;
  }
  std::cout << "Unrecognized playback status \"" << status << "\"" << std::endl;
  return PlaybackStatus::UNKNOWN;
}

#endif  // TOUCHEGG_MUSIC_CLIENT_PLAYBACK_STATUS_H
