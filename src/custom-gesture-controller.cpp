#include "custom-gesture-controller.h"

#include "gesture/gesture.h"

CustomGestureController::CustomGestureController(
    const PulseAudioAdapter &adapter, const WindowSystem &window_system)
    : adapter(adapter), window_system(window_system) {}

void CustomGestureController::onGestureBegin(std::unique_ptr<Gesture> gesture) {
  std::cout << "Gesture begin detected" << std::endl;
  std::cout << "\tGesture information:" << std::endl;
  std::cout << "\t\tFingers: " << gesture->fingers() << std::endl;
  std::cout << "\t\tType: " << gestureTypeToStr(gesture->type()) << std::endl;
  std::cout << "\t\tDirection: " << gestureDirectionToStr(gesture->direction())
            << std::endl;
}

void CustomGestureController::onGestureUpdate(
    std::unique_ptr<Gesture> gesture) {
  std::cout << "Gesture update detected (" << gesture->percentage() << "%)"
            << std::endl;
}

void CustomGestureController::onGestureEnd(
    std::unique_ptr<Gesture> /*gesture*/) {
  std::cout << "Gesture end detected" << std::endl;
}
