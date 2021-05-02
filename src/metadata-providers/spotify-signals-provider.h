#ifndef TOUCHEGG_MUSIC_CLIENT_SPOTIFY_SIGNALS_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_SPOTIFY_SIGNALS_PROVIDER_H

#include <giomm.h>
#include <glibmm.h>

#include <map>
#include <memory>
#include <string>

#include "metadata-providers/base-metadata-provider.h"
#include "metadata/metadata.h"

using VariantDict = std::map<std::string, Glib::VariantBase>;

class SpotifySignalsProvider : public BaseMetadataProvider,
                               public sigc::trackable {
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
  void onPropertiesChanged(
      const Glib::RefPtr<Gio::DBus::Connection>& connection,
      const Glib::ustring& sender_name, const Glib::ustring& object_path,
      const Glib::ustring& interface_name, const Glib::ustring& signal_name,
      const Glib::VariantContainerBase& parameters);
  static std::shared_ptr<const Metadata> parseMetadata(
      const VariantDict& metadata_dict, const std::string& playback_status_str);
  static inline std::string variantToString(const Glib::VariantBase& variant);
  static inline VariantDict variantToDict(const Glib::VariantBase& variant);

  Glib::RefPtr<Gio::DBus::Connection> dbus_connection;
  std::shared_ptr<const Metadata> metadata;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_SPOTIFY_SIGNALS_PROVIDER_H
