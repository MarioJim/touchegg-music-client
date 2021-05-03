#include "action/offset-volume-action.h"

OffsetVolumeAction::OffsetVolumeAction(
    const MetadataController& metadata_controller, PulseAudioAdapter& adapter,
    const WindowSystem& window_system, const WindowsConfig& windows_config)
    : metadata_controller(metadata_controller),
      adapter(adapter),
      window_system(window_system),
      windows_config(windows_config) {}

void OffsetVolumeAction::onGestureBegin(const Gesture& /*gesture*/) {
  volume_window = std::make_unique<VolumeWindow>(window_system, windows_config);
  metadata_window =
      std::make_unique<MetadataWindow>(window_system, windows_config);
  album_icon_window =
      std::make_unique<AlbumIconWindow>(window_system, windows_config);
}

void OffsetVolumeAction::onGestureUpdate(const Gesture& gesture) {
  bool is_reversed = gesture.direction() == GestureDirection::DOWN;
  double delta_percentage = gesture.percentage() - last_gesture_percentage;
  delta_percentage *= is_reversed ? -1 : 1;
  adapter.offset_volume(delta_percentage);

  double new_volume = adapter.get_volume();
  volume_window->render(new_volume);

  std::shared_ptr<const Metadata> metadata = metadata_controller.getMetadata();
  metadata_window->render(metadata);

  if (metadata != nullptr) {
    GdkPixbuf* album_icon = metadata->getAlbumIcon();
    album_icon_window->render(album_icon);
  }

  last_gesture_percentage = gesture.percentage();
}

void OffsetVolumeAction::onGestureEnd(const Gesture& gesture) {}
