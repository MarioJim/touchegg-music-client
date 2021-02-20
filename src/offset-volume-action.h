#ifndef TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
#define TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H

#include <memory>

#include "action.h"
#include "gesture/gesture.h"
#include "metadata/metadata-provider.h"
#include "pulseaudio-adapter.h"
#include "utils/color.h"
#include "window.h"

class OffsetVolumeAction : public Action {
 public:
  explicit OffsetVolumeAction(MetadataProvider &provider,
                              PulseAudioAdapter &adapter,
                              const WindowSystem &window_system);
  void onGestureBegin(const Gesture &gesture) override;
  void onGestureUpdate(const Gesture &gesture) override;
  void onGestureEnd(const Gesture &gesture) override;

 private:
  MetadataProvider &provider;
  PulseAudioAdapter &adapter;
  const WindowSystem &window_system;
  std::unique_ptr<Window> window = nullptr;
  double last_gesture_percentage = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
