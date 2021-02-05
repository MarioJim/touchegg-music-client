#include <iostream>
#include "actions/action-type.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << actionTypeToStr(ActionType::FULLSCREEN_WINDOW) << "\n";

    return 0;
}
