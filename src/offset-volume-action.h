#ifndef TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
#define TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H

#include "custom-action.h"
#include "gesture/gesture.h"
#include "pulseaudio-adapter.h"

class OffsetVolumeAction : public CustomAction {
 public:
  explicit OffsetVolumeAction(PulseAudioAdapter &adapter);
  void onGestureBegin(const Gesture &gesture) override;
  void onGestureUpdate(const Gesture &gesture) override;
  void onGestureEnd(const Gesture &gesture) override;

 private:
  PulseAudioAdapter &adapter;
  double lastGesturePercentage = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_OFFSET_VOLUME_ACTION_H
