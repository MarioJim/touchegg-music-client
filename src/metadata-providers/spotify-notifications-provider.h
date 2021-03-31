#ifndef TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H

#include <dbus/dbus.h>

#include <memory>
#include <shared_mutex>

#include "metadata-providers/base-metadata-provider.h"
#include "metadata/metadata.h"

class SpotifyNotificationsProvider : public BaseMetadataProvider {
 public:
  SpotifyNotificationsProvider();
  ~SpotifyNotificationsProvider();
  std::unique_ptr<Metadata> getMetadata() override;

 private:
  static DBusHandlerResult onNotificationReceived(
      [[maybe_unused]] DBusConnection *connection, DBusMessage *message,
      void *user_data);

  DBusConnection *connection{nullptr};
  std::shared_mutex metadata_mutex;
  std::unique_ptr<Metadata> metadata;

  const char *const kDBusBecomeMonitor{"BecomeMonitor"};
  const std::array<const char *, 1> kNotificationFilters = {
      "type='method_call',interface='org.freedesktop.Notifications',"
      "member='Notify',arg0='Spotify'"};
  const std::string kNotificationsPath = "/org/freedesktop/Notifications";
  const std::string kSeparator = " - ";
};

#endif  // TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H
