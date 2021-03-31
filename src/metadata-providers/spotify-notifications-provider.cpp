#include "metadata-providers/spotify-notifications-provider.h"

#include <iostream>
#include <thread>

SpotifyNotificationsProvider::SpotifyNotificationsProvider() {
  GError *error = nullptr;

  connection = g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, &error);
  if (connection == nullptr) {
    std::cerr << "SpotifyNotificationsProvider: Couldn't get DBus session bus\n"
              << "Error: " << error->message << std::endl;
    g_error_free(error);
    return;
  }

  g_dbus_connection_add_filter(
      connection, SpotifyNotificationsProvider::onNotificationReceived, this,
      nullptr);

  GDBusMessage *request = g_dbus_message_new_method_call(
      kDBusServiceDBus, kDBusPathDBus, kDBusInterfaceMonitoring,
      kDBusBecomeMonitor);

  GVariant *notification_filter = g_variant_new_string(kNotificationFilter);
  GVariant *filters_array =
      g_variant_new_array(G_VARIANT_TYPE_STRING, &notification_filter, 1);
  GVariant *flag = g_variant_new_uint32(0);
  std::array<GVariant *, 2> body_children = {filters_array, flag};
  GVariant *body = g_variant_new_tuple(body_children.data(), 2);
  g_dbus_message_set_body(request, body);

  GDBusMessage *response = g_dbus_connection_send_message_with_reply_sync(
      connection, request, G_DBUS_SEND_MESSAGE_FLAGS_NONE, -1, nullptr, nullptr,
      &error);

  if (response != nullptr) {
    // TODO: crashea este pex ?????????????????????????????????????????????
    g_object_unref(response);
  } else {
    std::cerr << "SpotifyNotificationsProvider: BecomeMonitor request "
                 "responded with an error\n"
              << "Error: " << error->message
              << std::endl;
    g_error_free(error);
  }
  g_object_unref(request);

//  std::thread receive_notifications_thread{[this]() {
//    while (dbus_connection_read_write_dispatch(this->connection, -1) == TRUE) {
//    }
//  }};
//  receive_notifications_thread.detach();
}

SpotifyNotificationsProvider::~SpotifyNotificationsProvider() {
  GError *error = nullptr;
  if (!g_dbus_connection_close_sync(connection, nullptr, &error)) {
    std::cerr << "SpotifyNotificationsProvider: BecomeMonitor request "
                 "responded with an error\n"
              << "Error: " << error->message
              << std::endl;
    g_error_free(error);

  }
}

std::unique_ptr<Metadata> SpotifyNotificationsProvider::getMetadata() {
  std::shared_lock lock(metadata_mutex);
  if (metadata == nullptr) {
    return nullptr;
  }
  return std::make_unique<Metadata>(*metadata);
}

GDBusMessage *SpotifyNotificationsProvider::onNotificationReceived(
    GDBusConnection * /*connection*/, GDBusMessage *message, gboolean /*incoming*/,
    gpointer user_data) {
  std::cout << g_dbus_message_print(message, 0) << std::endl;
//  if (dbus_message_get_type(message) != DBUS_MESSAGE_TYPE_METHOD_CALL) {
//    return DBUS_HANDLER_RESULT_HANDLED;
//  }

  auto *provider = static_cast<SpotifyNotificationsProvider *>(user_data);
//  std::string path{dbus_message_get_path(message)};
//  if (path != provider->kNotificationsPath) {
//    return DBUS_HANDLER_RESULT_HANDLED;
//  }
//
//  DBusMessageIter iter;
//  dbus_message_iter_init(message, &iter);  // app_name
//  dbus_message_iter_next(&iter);           // replaces_id
//  dbus_message_iter_next(&iter);           // app_icon
//
//  dbus_message_iter_next(&iter);  // summary (song name)
//  char *song_str = nullptr;
//  dbus_message_iter_get_basic(&iter, &song_str);
//  std::string song{song_str};
//
//  dbus_message_iter_next(&iter);  // body (artist and album names)
//  char *artist_album_str = nullptr;
//  dbus_message_iter_get_basic(&iter, &artist_album_str);
//  std::string artist_album{artist_album_str};
//  std::string::size_type separator_position =
//      artist_album.find(provider->kSeparator);
//  std::string artist = artist_album.substr(0, separator_position);
//  std::string album;
//  if (separator_position != std::string::npos) {
//    album =
//        artist_album.substr(separator_position + provider->kSeparator.size());
//  }
//
//  std::unique_lock lock(provider->metadata_mutex);
//  provider->metadata =
//      std::make_unique<Metadata>(song, album, artist, PlaybackStatus::UNKNOWN);
//  lock.unlock();
//
//  return DBUS_HANDLER_RESULT_HANDLED;
  return nullptr;
}
