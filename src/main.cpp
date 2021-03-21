#include "controllers/gesture-controller.h"
#include "controllers/metadata-controller.h"
#include "daemon/daemon-client.h"
#include "pulseaudio/pulseaudio-adapter.h"
#include "window-system/x11.h"

int main() {
  MetadataController metadata_controller;
  PulseAudioAdapter adapter;
  X11 window_system;

  GestureController gesture_controller(metadata_controller, adapter,
                                       window_system);
  DaemonClient client{&gesture_controller};
  client.run();
  return 0;
}
