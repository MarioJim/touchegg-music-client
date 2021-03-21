#ifndef TOUCHEGG_MUSIC_CLIENT_SPOTIFY_METADATA_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_SPOTIFY_METADATA_PROVIDER_H

#include <gio/gio.h>

#include <memory>
#include <string>

#include "base-metadata-provider.h"
#include "metadata/metadata.h"

class SpotifyMetadataProvider : public BaseMetadataProvider {
 public:
  SpotifyMetadataProvider();
  ~SpotifyMetadataProvider();
  std::unique_ptr<Metadata> getMetadata() override;

 private:
  bool initSpotifyProxy();

  bool isSpotifyDBusConnected();
  GVariant *fetchMetadataGVariant();
  GVariant *fetchPlaybackStatusGVariant();
  static std::unique_ptr<Metadata> parseMetadataFromGVariant(
      GVariant *metadata_dict, GVariant *playback_status_variant);

  GDBusProxy *spotify_proxy{nullptr};

  const char *const kSpotifyDBusName{"org.mpris.MediaPlayer2.spotify"};
};

#endif  // TOUCHEGG_MUSIC_CLIENT_SPOTIFY_METADATA_PROVIDER_H
