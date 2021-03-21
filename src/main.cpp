#include <memory>

#include "controllers/gesture-controller.h"
#include "daemon/daemon-client.h"
#include "metadata-providers/base-metadata-provider.h"
#include "pulseaudio/pulseaudio-adapter.h"
#include "window-system/x11.h"

int main() {
  std::unique_ptr<BaseMetadataProvider> provider =
      BaseMetadataProvider::selectMetadataProvider();
  PulseAudioAdapter adapter;
  X11 window_system;

  GestureController gesture_controller(*provider, adapter, window_system);
  DaemonClient client{&gesture_controller};
  client.run();
  return 0;
}
