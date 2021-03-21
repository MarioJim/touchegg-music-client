#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_CONTROLLER_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_CONTROLLER_H

#include <vector>

#include "metadata-providers/base-metadata-provider.h"
#include "metadata/metadata.h"

class MetadataController {
 public:
  explicit MetadataController();
  std::unique_ptr<Metadata> getMetadata();

 private:
  std::vector<std::unique_ptr<BaseMetadataProvider>> providers;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_CONTROLLER_H
