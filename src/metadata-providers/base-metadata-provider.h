#ifndef TOUCHEGG_MUSIC_CLIENT_BASE_METADATA_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_BASE_METADATA_PROVIDER_H

#include <memory>

#include "metadata/metadata.h"

class BaseMetadataProvider {
 public:
  virtual std::shared_ptr<const Metadata> getMetadata() = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_BASE_METADATA_PROVIDER_H
