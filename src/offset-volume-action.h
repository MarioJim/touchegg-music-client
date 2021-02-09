#ifndef TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
#define TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H

#include "custom-action.h"
#include "gesture/gesture.h"
#include "pulseaudio-adapter.h"
#include "utils/color.h"
#include "volume-window.h"

class OffsetVolumeAction : public CustomAction {
 public:
  explicit OffsetVolumeAction(PulseAudioAdapter &adapter,
                              const WindowSystem &window_system);
  void onGestureBegin(const Gesture &gesture) override;
  void onGestureUpdate(const Gesture &gesture) override;
  void onGestureEnd(const Gesture &gesture) override;

 private:
  PulseAudioAdapter &adapter;
  const WindowSystem &window_system;
  std::unique_ptr<VolumeWindow> window = nullptr;
  double last_gesture_percentage = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
