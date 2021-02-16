#include "offset-volume-action.h"

OffsetVolumeAction::OffsetVolumeAction(MetadataProvider& provider,
                                       PulseAudioAdapter& adapter,
                                       const WindowSystem& window_system)
    : provider(provider), adapter(adapter), window_system(window_system) {}

void OffsetVolumeAction::onGestureBegin(const Gesture& /*gesture*/) {
  window = std::make_unique<Window>(window_system);
}

void OffsetVolumeAction::onGestureUpdate(const Gesture& gesture) {
  bool is_reversed = gesture.direction() == GestureDirection::DOWN;
  double delta_percentage = gesture.percentage() - last_gesture_percentage;
  delta_percentage *= is_reversed ? -1 : 1;
  adapter.offset_volume(delta_percentage);

  double new_volume = adapter.get_volume();
  auto metadata = provider.getMetadata();
  window->render(new_volume, std::move(metadata));

  last_gesture_percentage = gesture.percentage();
}

void OffsetVolumeAction::onGestureEnd(const Gesture& gesture) {}
