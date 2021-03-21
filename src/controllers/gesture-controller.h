#ifndef TOUCHEGG_MUSIC_CLIENT_GESTURE_CONTROLLER_H
#define TOUCHEGG_MUSIC_CLIENT_GESTURE_CONTROLLER_H

#include <memory>

#include "action/action.h"
#include "gesture-controller/gesture-controller-delegate.h"
#include "metadata-providers/base-metadata-provider.h"
#include "pulseaudio/pulseaudio-adapter.h"
#include "window-system/window-system.h"

class GestureController : public GestureControllerDelegate {
 public:
  explicit GestureController(BaseMetadataProvider &provider,
                             PulseAudioAdapter &adapter,
                             const WindowSystem &window_system);
  void onGestureBegin(std::unique_ptr<Gesture> gesture) override;
  void onGestureUpdate(std::unique_ptr<Gesture> gesture) override;
  void onGestureEnd(std::unique_ptr<Gesture> gesture) override;

 private:
  BaseMetadataProvider &provider;
  PulseAudioAdapter &adapter;
  const WindowSystem &window_system;

  std::unique_ptr<Action> action = nullptr;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_GESTURE_CONTROLLER_H
