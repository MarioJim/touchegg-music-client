#ifndef TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
#define TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H

#include <memory>

#include "action/action.h"
#include "controllers/metadata-controller.h"
#include "gesture/gesture.h"
#include "pulseaudio/pulseaudio-adapter.h"
#include "utils/color.h"
#include "window/window.h"

class OffsetVolumeAction : public Action {
 public:
  explicit OffsetVolumeAction(const MetadataController &metadata_controller,
                              PulseAudioAdapter &adapter,
                              const WindowSystem &window_system);
  void onGestureBegin(const Gesture &gesture) override;
  void onGestureUpdate(const Gesture &gesture) override;
  void onGestureEnd(const Gesture &gesture) override;

 private:
  const MetadataController &metadata_controller;
  PulseAudioAdapter &adapter;
  const WindowSystem &window_system;
  std::unique_ptr<Window> window = nullptr;
  double last_gesture_percentage = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H