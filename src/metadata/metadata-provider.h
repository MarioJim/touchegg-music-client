#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_PROVIDER_H

#include <memory>

#include "metadata/metadata.h"

class MetadataProvider {
 public:
  virtual std::unique_ptr<Metadata> getMetadata() = 0;
  static std::unique_ptr<MetadataProvider> selectMetadataProvider();
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_PROVIDER_H
