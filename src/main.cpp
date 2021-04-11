#include "controllers/gesture-controller.h"
#include "controllers/metadata-controller.h"
#include "daemon/daemon-client.h"
#include "pulseaudio/pulseaudio-adapter.h"
#include "window-system/x11.h"
#include "windows/windows-config.h"

int main() {
  MetadataController metadata_controller;
  PulseAudioAdapter adapter;
  X11 window_system;
  WindowsConfig windows_config;

  GestureController gesture_controller(metadata_controller, adapter,
                                       window_system, windows_config);
  DaemonClient client{&gesture_controller};
  client.run();
  return 0;
}
