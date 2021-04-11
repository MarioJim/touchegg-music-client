#include "action/offset-volume-action.h"

#include <utility>

OffsetVolumeAction::OffsetVolumeAction(
    const MetadataController& metadata_controller, PulseAudioAdapter& adapter,
    const WindowSystem& window_system)
    : metadata_controller(metadata_controller),
      adapter(adapter),
      window_system(window_system) {}

void OffsetVolumeAction::onGestureBegin(const Gesture& /*gesture*/) {
  metadata_window = std::make_unique<MetadataWindow>(window_system);
  album_icon_window = std::make_unique<AlbumIconWindow>(window_system);
}

void OffsetVolumeAction::onGestureUpdate(const Gesture& gesture) {
  bool is_reversed = gesture.direction() == GestureDirection::DOWN;
  double delta_percentage = gesture.percentage() - last_gesture_percentage;
  delta_percentage *= is_reversed ? -1 : 1;
  adapter.offset_volume(delta_percentage);

  double new_volume = adapter.get_volume();
  std::shared_ptr<const Metadata> metadata = metadata_controller.getMetadata();
  GdkPixbuf* album_icon = metadata->album_icon;
  metadata_window->render(new_volume, std::move(metadata));
  album_icon_window->render(album_icon);

  last_gesture_percentage = gesture.percentage();
}

void OffsetVolumeAction::onGestureEnd(const Gesture& gesture) {}
