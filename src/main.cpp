#include <window-system/x11.h>
#include <daemon/daemon-client.h>

#include <custom-gesture-controller.h>

int main() {
    X11 windowSystem;
    CustomGestureController gestureController(windowSystem);
    DaemonClient daemonClient{&gestureController};
    daemonClient.run();
    return 0;
}
