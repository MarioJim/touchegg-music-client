#include "metadata/metadata-provider.h"

#include "metadata/spotify-metadata-provider.h"

std::unique_ptr<MetadataProvider> MetadataProvider::selectMetadataProvider() {
  return std::make_unique<SpotifyMetadataProvider>();
}
