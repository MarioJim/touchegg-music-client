#ifndef TOUCHEGG_MUSIC_CLIENT_CUSTOM_GESTURE_CONTROLLER_H
#define TOUCHEGG_MUSIC_CLIENT_CUSTOM_GESTURE_CONTROLLER_H

#include "gesture-controller/gesture-controller-delegate.h"
#include "pulseaudio-adapter.h"
#include "window-system/window-system.h"

class CustomGestureController : public GestureControllerDelegate {
 public:
  explicit CustomGestureController(const PulseAudioAdapter &adapter,
                                   const WindowSystem &window_system);
  ~CustomGestureController() override = default;
  void onGestureBegin(std::unique_ptr<Gesture> gesture) override;
  void onGestureUpdate(std::unique_ptr<Gesture> gesture) override;
  void onGestureEnd(std::unique_ptr<Gesture> gesture) override;

 private:
  const PulseAudioAdapter &adapter;
  const WindowSystem &window_system;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_CUSTOM_GESTURE_CONTROLLER_H
