include(FetchContent)

FetchContent_Declare(touchegg
        GIT_REPOSITORY https://github.com/JoseExposito/touchegg.git
        GIT_TAG 2.0.8
        GIT_SHALLOW ON
        GIT_PROGRESS ON
)
list(APPEND CMAKE_MODULE_PATH "${touchegg_SOURCE_DIR}/cmake/Modules")
FetchContent_MakeAvailable(touchegg)

set(TOUCHEGG_SOURCE_FILES
        "${touchegg_SOURCE_DIR}/src/actions/action-direction.h"

        "${touchegg_SOURCE_DIR}/src/daemon/daemon-client.cpp"
        "${touchegg_SOURCE_DIR}/src/daemon/daemon-client.h"
        "${touchegg_SOURCE_DIR}/src/daemon/dbus.h"

        "${touchegg_SOURCE_DIR}/src/gesture/device-type.h"
        "${touchegg_SOURCE_DIR}/src/gesture/gesture.h"
        "${touchegg_SOURCE_DIR}/src/gesture/gesture-direction.h"
        "${touchegg_SOURCE_DIR}/src/gesture/gesture-type.h"

        "${touchegg_SOURCE_DIR}/src/gesture-controller/gesture-controller-delegate.h"

        "${touchegg_SOURCE_DIR}/src/utils/color.cpp"
        "${touchegg_SOURCE_DIR}/src/utils/color.h"
        "${touchegg_SOURCE_DIR}/src/utils/logger.cpp"
        "${touchegg_SOURCE_DIR}/src/utils/logger.h"
        "${touchegg_SOURCE_DIR}/src/utils/rectangle.h"

        "${touchegg_SOURCE_DIR}/src/window-system/cairo-surface.h"
        "${touchegg_SOURCE_DIR}/src/window-system/window-system.h"
        "${touchegg_SOURCE_DIR}/src/window-system/x11.cpp"
        "${touchegg_SOURCE_DIR}/src/window-system/x11.h"
        "${touchegg_SOURCE_DIR}/src/window-system/x11-cairo-surface.cpp"
        "${touchegg_SOURCE_DIR}/src/window-system/x11-cairo-surface.h")
