#ifndef TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H
#define TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H

#include <dbus/dbus.h>
#include <gio/gio.h>

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
  static GDBusMessage *onNotificationReceived(GDBusConnection *connection,
                                              GDBusMessage *message,
                                              gboolean incoming,
                                              gpointer user_data);

  GDBusConnection *connection{nullptr};
  std::shared_mutex metadata_mutex;
  std::unique_ptr<Metadata> metadata;

  const gchar *const kDBusServiceDBus{"org.freedesktop.DBus"};
  const gchar *const kDBusPathDBus{"/org/freedesktop/DBus"};
  const gchar *const kDBusInterfaceMonitoring{
      "org.freedesktop.DBus.Monitoring"};
  const gchar *const kDBusBecomeMonitor{"BecomeMonitor"};
  const gchar *const kNotificationFilter{
      "type='method_call',interface='org.freedesktop.Notifications',"
      "member='Notify',arg0='Spotify'"};
  const std::string kNotificationsPath = "/org/freedesktop/Notifications";
  const std::string kSeparator = " - ";
};

#endif  // TOUCHEGG_MUSIC_CLIENT_SPOTIFY_NOTIFICATIONS_PROVIDER_H
