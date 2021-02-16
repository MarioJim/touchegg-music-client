#include "daemon/daemon-client.h"
#include "gesture-controller.h"
#include "metadata/metadata-provider.h"
#include "pulseaudio-adapter.h"
#include "window-system/x11.h"

int main() {
  std::unique_ptr<MetadataProvider> provider =
      MetadataProvider::selectMetadataProvider();
  PulseAudioAdapter adapter;
  X11 window_system;

  GestureController gesture_controller(*provider, adapter, window_system);
  DaemonClient client{&gesture_controller};
  client.run();
  return 0;
}
