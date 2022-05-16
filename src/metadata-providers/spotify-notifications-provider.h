#ifndef TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H

#include <dbus/dbus.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <array>
#include <memory>
#include <optional>
#include <string_view>

#include "metadata-providers/base-metadata-provider.h"
#include "metadata/metadata.h"

class SpotifyNotificationsProvider : public BaseMetadataProvider {
 public:
  SpotifyNotificationsProvider();
  virtual ~SpotifyNotificationsProvider();

  SpotifyNotificationsProvider(const SpotifyNotificationsProvider &other) =
      delete;
  SpotifyNotificationsProvider &operator=(
      const SpotifyNotificationsProvider &other) = delete;
  SpotifyNotificationsProvider(SpotifyNotificationsProvider &&other) noexcept =
      delete;
  SpotifyNotificationsProvider &operator=(
      SpotifyNotificationsProvider &&other) noexcept = delete;

  std::shared_ptr<const Metadata> getMetadata() override;

 private:
  static DBusHandlerResult onNotificationReceived(DBusConnection *connection,
                                                  DBusMessage *message,
                                                  void *user_data);
  static std::optional<DBusMessageIter> findImageDataHint(
      DBusMessageIter *hints_iter);
  static GdkPixbuf *parseIconFromDBusVariant(DBusMessageIter *variant_iter);

  DBusConnection *connection{nullptr};
  std::shared_ptr<const Metadata> metadata;

  const char *const kDBusBecomeMonitor{"BecomeMonitor"};
  const std::array<const char *, 1> kNotificationFilters{
      "type='method_call',interface='org.freedesktop.Notifications',"
      "member='Notify',arg0='Spotify'"};
  static constexpr const std::string_view kNotificationsPath =
      "/org/freedesktop/Notifications";
  static constexpr const std::string_view kSeparator = " - ";
  static constexpr const std::string_view kImageDataKey = "image-data";
};

#endif  // TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H
