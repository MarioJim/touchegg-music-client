#include "gesture-controller.h"

#include "gesture/gesture.h"
#include "offset-volume-action.h"

GestureController::GestureController(MetadataProvider &provider,
                                     PulseAudioAdapter &adapter,
                                     const WindowSystem &window_system)
    : provider(provider), adapter(adapter), window_system(window_system) {}

void GestureController::onGestureBegin(std::unique_ptr<Gesture> gesture) {
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
        action = std::make_unique<OffsetVolumeAction>(provider, adapter,
                                                      window_system);
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

void GestureController::onGestureUpdate(std::unique_ptr<Gesture> gesture) {
  if (action != nullptr) {
    std::cout << "Gesture update detected (" << gesture->percentage() << "%)"
              << std::endl;
    action->onGestureUpdate(*gesture);
  }
}

void GestureController::onGestureEnd(std::unique_ptr<Gesture> gesture) {
  std::cout << "Gesture end detected" << std::endl;
  if (action != nullptr) {
    action->onGestureEnd(*gesture);
  }
  this->action.reset();
}
