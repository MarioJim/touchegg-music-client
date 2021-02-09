#include "custom-gesture-controller.h"

#include "gesture/gesture.h"
#include "offset-volume-action.h"

CustomGestureController::CustomGestureController(
    PulseAudioAdapter &adapter, const WindowSystem &window_system)
    : adapter(adapter), window_system(window_system) {}

void CustomGestureController::onGestureBegin(std::unique_ptr<Gesture> gesture) {
  std::cout << "Gesture begin detected" << std::endl;
  std::cout << "\tGesture information:" << std::endl;
  std::cout << "\t\tFingers: " << gesture->fingers() << std::endl;
  std::cout << "\t\tType: " << gestureTypeToStr(gesture->type()) << std::endl;
  std::cout << "\t\tDirection: " << gestureDirectionToStr(gesture->direction())
            << std::endl;

  if (gesture->type() == GestureType::SWIPE && gesture->fingers() == 4) {
    switch (gesture->direction()) {
      case GestureDirection::UP:
      case GestureDirection::DOWN:
        action = std::make_unique<OffsetVolumeAction>(adapter, window_system);
        break;
      case GestureDirection::LEFT:
      case GestureDirection::RIGHT:
        // TODO: Implement song changing action
        break;
      case GestureDirection::UNKNOWN:
      case GestureDirection::IN:
      case GestureDirection::OUT:
        break;
    }
  }
  if (action != nullptr) {
    action->onGestureBegin(*gesture);
  }
}

void CustomGestureController::onGestureUpdate(
    std::unique_ptr<Gesture> gesture) {
  std::cout << "Gesture update detected (" << gesture->percentage() << "%)"
            << std::endl;
  if (action != nullptr) {
    action->onGestureUpdate(*gesture);
  }
}

void CustomGestureController::onGestureEnd(std::unique_ptr<Gesture> gesture) {
  std::cout << "Gesture end detected" << std::endl;
  if (action != nullptr) {
    action->onGestureEnd(*gesture);
  }
  this->action.reset();
}
