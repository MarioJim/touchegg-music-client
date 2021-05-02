#ifndef TOUCHEGG_MUSIC_CLIENT_SPOTIFY_SIGNALS_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_SPOTIFY_SIGNALS_PROVIDER_H

#include <gio/gio.h>

#include <memory>

#include "metadata-providers/base-metadata-provider.h"
#include "metadata/metadata.h"

class SpotifySignalsProvider : public BaseMetadataProvider {
 public:
  SpotifySignalsProvider();
  ~SpotifySignalsProvider();

  SpotifySignalsProvider(const SpotifySignalsProvider& other) = delete;
  SpotifySignalsProvider& operator=(const SpotifySignalsProvider& other) =
      delete;
  SpotifySignalsProvider(SpotifySignalsProvider&& other) noexcept = delete;
  SpotifySignalsProvider& operator=(SpotifySignalsProvider&& other) noexcept =
      delete;

  std::shared_ptr<const Metadata> getMetadata() override;

 private:
  bool initSpotifyProxy();
  static void onPropertiesChanged(GDBusConnection* connection,
                                  const gchar* sender_name,
                                  const gchar* object_path,
                                  const gchar* interface_name,
                                  const gchar* signal_name,
                                  GVariant* parameters, gpointer user_data);
  static std::shared_ptr<const Metadata> metadataFromGVariant(
      GVariant* metadata_dict, GVariant* playback_status_variant);

  GDBusConnection* dbus_connection{nullptr};
  std::shared_ptr<const Metadata> metadata;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_SPOTIFY_SIGNALS_PROVIDER_H
