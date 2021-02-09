#include "offset-volume-action.h"

OffsetVolumeAction::OffsetVolumeAction(PulseAudioAdapter& adapter)
    : adapter(adapter) {}

void OffsetVolumeAction::onGestureBegin(const Gesture& gesture) {}

void OffsetVolumeAction::onGestureUpdate(const Gesture& gesture) {
  bool is_reversed = gesture.direction() == GestureDirection::DOWN;
  double delta_percentage = gesture.percentage() - lastGesturePercentage;
  delta_percentage *= is_reversed ? -1 : 1;
  adapter.offset_volume(delta_percentage);
  lastGesturePercentage = gesture.percentage();
}

void OffsetVolumeAction::onGestureEnd(const Gesture& gesture) {}
