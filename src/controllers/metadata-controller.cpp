#include "controllers/metadata-controller.h"

#include "metadata-providers/spotify-notifications-provider.h"
#include "metadata-providers/spotify-signals-provider.h"

MetadataController::MetadataController() {
  providers.push_back(std::make_unique<SpotifyNotificationsProvider>());
  providers.push_back(std::make_unique<SpotifySignalsProvider>());
}

std::shared_ptr<const Metadata> MetadataController::getMetadata() const {
  for (const auto& provider : providers) {
    std::shared_ptr<const Metadata> metadata = provider->getMetadata();
    if (metadata != nullptr) {
      return metadata;
    }
  }
  return nullptr;
}
