#include "daemon/daemon-client.h"
#include "gesture-controller.h"
#include "pulseaudio-adapter.h"
#include "window-system/x11.h"

int main() {
  PulseAudioAdapter adapter;
  X11 window_system;

  GestureController gesture_controller(adapter, window_system);
  DaemonClient client{&gesture_controller};
  client.run();
  return 0;
}
