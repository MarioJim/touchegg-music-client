#ifndef TOUCHEGG_MUSIC_CLIENT_SPOTIFY_METADATA_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_SPOTIFY_METADATA_PROVIDER_H

#include <gio/gio.h>

#include "metadata/metadata-provider.h"
#include "metadata/metadata.h"

class SpotifyMetadataProvider : public MetadataProvider {
 public:
  explicit SpotifyMetadataProvider();
  ~SpotifyMetadataProvider();
  std::unique_ptr<Metadata> getMetadata() override;

 private:
  bool initSpotifyProxy();

  bool isSpotifyDBusConnected();
  static std::unique_ptr<Metadata> parseMetadataFromGVariant(
      GVariant *metadata_dict);

  GDBusProxy *spotify_proxy{nullptr};

  const char *const kSpotifyDBusName{"org.mpris.MediaPlayer2.spotify"};
};

#endif  // TOUCHEGG_MUSIC_CLIENT_SPOTIFY_METADATA_PROVIDER_H