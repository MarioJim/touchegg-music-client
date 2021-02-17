#ifndef TOUCHEGG_MUSIC_CLIENT_PLAYBACK_STATUS_H
#define TOUCHEGG_MUSIC_CLIENT_PLAYBACK_STATUS_H

enum class PlaybackStatus {
  PLAYING,
  PAUSED,
  STOPPED,
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
  return PlaybackStatus::STOPPED;
}

inline std::string playbackStatusToString(const PlaybackStatus status) {
  switch (status) {
    case PlaybackStatus::PLAYING:
      return "Playing";
    case PlaybackStatus::PAUSED:
      return "Paused";
    case PlaybackStatus::STOPPED:
      return "Stopped";
  }
  std::cout << "Unrecognized playback status passed to playbackStatusToString"
            << std::endl;
  return "";
}

#endif  // TOUCHEGG_MUSIC_CLIENT_PLAYBACK_STATUS_H
