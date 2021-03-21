#include "base-metadata-provider.h"

#include "spotify-metadata-provider.h"

std::unique_ptr<BaseMetadataProvider>
BaseMetadataProvider::selectMetadataProvider() {
  return std::make_unique<SpotifyMetadataProvider>();
}
