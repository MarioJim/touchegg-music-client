#ifndef TOUCHEGG_MUSIC_CLIENT_SPOTIFY_POLLING_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_SPOTIFY_POLLING_PROVIDER_H

#include <gio/gio.h>

#include <memory>

#include "metadata-providers/base-metadata-provider.h"
#include "metadata/metadata.h"

class SpotifyPollingProvider : public BaseMetadataProvider {
 public:
  SpotifyPollingProvider();
  ~SpotifyPollingProvider();

  SpotifyPollingProvider(const SpotifyPollingProvider& other) = delete;
  SpotifyPollingProvider& operator=(const SpotifyPollingProvider& other) =
      delete;
  SpotifyPollingProvider(SpotifyPollingProvider&& other) noexcept = delete;
  SpotifyPollingProvider& operator=(SpotifyPollingProvider&& other) noexcept =
      delete;

  std::shared_ptr<const Metadata> getMetadata() override;

 private:
  bool initSpotifyProxy();
  bool isSpotifyDBusConnected();
  GVariant* fetchMetadataGVariant();
  GVariant* fetchPlaybackStatusGVariant();
  static std::shared_ptr<const Metadata> metadataFromGVariant(
      GVariant* metadata_dict, GVariant* playback_status_variant);

  GDBusProxy* spotify_proxy{nullptr};
  std::shared_ptr<const Metadata> metadata;

  const char* const kSpotifyDBusName{"org.mpris.MediaPlayer2.spotify"};
};

#endif  // TOUCHEGG_MUSIC_CLIENT_SPOTIFY_POLLING_PROVIDER_H
