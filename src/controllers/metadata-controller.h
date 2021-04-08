#ifndef TOUCHEGG_MUSIC_CLIENT_METADATA_CONTROLLER_H
#define TOUCHEGG_MUSIC_CLIENT_METADATA_CONTROLLER_H

#include <memory>
#include <vector>

#include "metadata-providers/base-metadata-provider.h"
#include "metadata/metadata.h"

class MetadataController {
 public:
  MetadataController();
  [[nodiscard]] std::shared_ptr<const Metadata> getMetadata() const;

 private:
  std::vector<std::unique_ptr<BaseMetadataProvider>> providers;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_METADATA_CONTROLLER_H
