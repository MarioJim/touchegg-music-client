#include "controllers/metadata-controller.h"

#include "metadata-providers/spotify-polling-provider.h"

MetadataController::MetadataController() {
  providers.push_back(std::make_unique<SpotifyPollingProvider>());
}

std::unique_ptr<Metadata> MetadataController::getMetadata() const {
  for (const auto& provider : providers) {
    std::unique_ptr<Metadata> metadata = provider->getMetadata();
    if (metadata != nullptr) {
      return metadata;
    }
  }
  return nullptr;
}
