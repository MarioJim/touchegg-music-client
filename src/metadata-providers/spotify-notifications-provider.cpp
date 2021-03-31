#include "metadata-providers/spotify-notifications-provider.h"

#include <iostream>
#include <thread>

SpotifyNotificationsProvider::SpotifyNotificationsProvider() {
  DBusError error;
  dbus_error_init(&error);

  connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
  if (connection == nullptr) {
    std::cerr << "SpotifyNotificationsProvider: Couldn't get DBus session bus"
              << std::endl;
    return;
  }

  dbus_bool_t could_add_filter = dbus_connection_add_filter(
      connection, SpotifyNotificationsProvider::onNotificationReceived, this,
      nullptr);
  if (could_add_filter == FALSE) {
    std::cerr
        << "SpotifyNotificationsProvider: Connection couldn't add a filter"
        << std::endl;
    return;
  }

  DBusMessage *request = dbus_message_new_method_call(
      DBUS_SERVICE_DBUS, DBUS_PATH_DBUS, DBUS_INTERFACE_MONITORING,
      kDBusBecomeMonitor);

  DBusMessageIter appender;
  dbus_message_iter_init_append(request, &appender);
  DBusMessageIter array_appender;
  dbus_message_iter_open_container(&appender, DBUS_TYPE_ARRAY, "s",
                                   &array_appender);
  dbus_message_iter_append_basic(&array_appender, DBUS_TYPE_STRING,
                                 &kNotificationFilters[0]);
  dbus_message_iter_close_container(&appender, &array_appender);

  dbus_uint32_t flags = 0;
  dbus_message_iter_append_basic(&appender, DBUS_TYPE_UINT32, &flags);

  DBusMessage *response = dbus_connection_send_with_reply_and_block(
      connection, request, -1, &error);
  if (response != nullptr) {
    dbus_message_unref(response);
  } else {
    std::cerr << "SpotifyNotificationsProvider: BecomeMonitor request "
                 "responded with an error"
              << std::endl;
    dbus_error_free(&error);
  }
  dbus_message_unref(request);

  std::thread receive_notifications_thread{[this]() {
    while (dbus_connection_read_write_dispatch(this->connection, -1) == TRUE) {
    }
  }};
  receive_notifications_thread.detach();
}

SpotifyNotificationsProvider::~SpotifyNotificationsProvider() {
  dbus_connection_close(connection);
}

std::unique_ptr<Metadata> SpotifyNotificationsProvider::getMetadata() {
  std::shared_lock lock(metadata_mutex);
  if (metadata == nullptr) {
    return nullptr;
  }
  return std::make_unique<Metadata>(*metadata);
}

DBusHandlerResult SpotifyNotificationsProvider::onNotificationReceived(
    [[maybe_unused]] DBusConnection *connection, DBusMessage *message,
    void *user_data) {
  if (dbus_message_get_type(message) != DBUS_MESSAGE_TYPE_METHOD_CALL) {
    return DBUS_HANDLER_RESULT_HANDLED;
  }

  auto *provider = static_cast<SpotifyNotificationsProvider *>(user_data);
  std::string path{dbus_message_get_path(message)};
  if (path != provider->kNotificationsPath) {
    return DBUS_HANDLER_RESULT_HANDLED;
  }

  DBusMessageIter iter;
  dbus_message_iter_init(message, &iter);  // app_name
  dbus_message_iter_next(&iter);           // replaces_id
  dbus_message_iter_next(&iter);           // app_icon

  dbus_message_iter_next(&iter);  // summary (song name)
  char *song_str = nullptr;
  dbus_message_iter_get_basic(&iter, &song_str);
  std::string song{song_str};

  dbus_message_iter_next(&iter);  // body (artist and album names)
  char *artist_album_str = nullptr;
  dbus_message_iter_get_basic(&iter, &artist_album_str);
  std::string artist_album{artist_album_str};
  std::string::size_type separator_position =
      artist_album.find(provider->kSeparator);
  std::string artist = artist_album.substr(0, separator_position);
  std::string album;
  if (separator_position != std::string::npos) {
    album =
        artist_album.substr(separator_position + provider->kSeparator.size());
  }

  std::unique_lock lock(provider->metadata_mutex);
  provider->metadata =
      std::make_unique<Metadata>(song, album, artist, PlaybackStatus::UNKNOWN);
  lock.unlock();

  return DBUS_HANDLER_RESULT_HANDLED;
}
