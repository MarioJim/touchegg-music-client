#include "metadata-controller.h"

#include "metadata-providers/spotify-metadata-provider.h"

MetadataController::MetadataController() {
  providers.push_back(std::make_unique<SpotifyMetadataProvider>());
}

std::unique_ptr<Metadata> MetadataController::getMetadata() {
  for (auto& provider : providers) {
    std::unique_ptr<Metadata> metadata = provider->getMetadata();
    if (metadata != nullptr) {
      return metadata;
    }
  }
  return nullptr;
}
