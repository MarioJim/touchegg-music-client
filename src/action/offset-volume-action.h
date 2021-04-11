#ifndef TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
#define TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H

#include <windows/volume-window.h>

#include <memory>

#include "action/action.h"
#include "controllers/metadata-controller.h"
#include "gesture/gesture.h"
#include "pulseaudio/pulseaudio-adapter.h"
#include "windows/album-icon-window.h"
#include "windows/metadata-window.h"
#include "windows/windows-config.h"

class OffsetVolumeAction : public Action {
 public:
  explicit OffsetVolumeAction(const MetadataController &metadata_controller,
                              PulseAudioAdapter &adapter,
                              const WindowSystem &window_system,
                              const WindowsConfig &windows_config);
  void onGestureBegin(const Gesture &gesture) override;
  void onGestureUpdate(const Gesture &gesture) override;
  void onGestureEnd(const Gesture &gesture) override;

 private:
  const MetadataController &metadata_controller;
  PulseAudioAdapter &adapter;
  const WindowSystem &window_system;
  const WindowsConfig &windows_config;

  std::unique_ptr<VolumeWindow> volume_window{nullptr};
  std::unique_ptr<MetadataWindow> metadata_window{nullptr};
  std::unique_ptr<AlbumIconWindow> album_icon_window{nullptr};

  double last_gesture_percentage = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
