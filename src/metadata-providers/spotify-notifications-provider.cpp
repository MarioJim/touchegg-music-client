#include "metadata-providers/spotify-notifications-provider.h"

#include <glib-object.h>

#include <iostream>
#include <string>
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

std::shared_ptr<const Metadata> SpotifyNotificationsProvider::getMetadata() {
  return atomic_load(&metadata);
}

DBusHandlerResult SpotifyNotificationsProvider::onNotificationReceived(
    DBusConnection * /*connection*/, DBusMessage *message, void *user_data) {
  if (dbus_message_get_type(message) != DBUS_MESSAGE_TYPE_METHOD_CALL) {
    return DBUS_HANDLER_RESULT_HANDLED;
  }

  auto *provider = static_cast<SpotifyNotificationsProvider *>(user_data);
  std::string path{dbus_message_get_path(message)};
  if (path != kNotificationsPath) {
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
  std::string::size_type separator_position = artist_album.find(kSeparator);
  std::string artist = artist_album.substr(0, separator_position);
  std::string album;
  if (separator_position != std::string::npos) {
    album = artist_album.substr(separator_position + kSeparator.size());
  }

  dbus_message_iter_next(&iter);  // actions

  dbus_message_iter_next(&iter);  // hints (album album_icon)
  DBusMessageIter hints_iter;
  dbus_message_iter_recurse(&iter, &hints_iter);
  GdkPixbuf *album_icon = nullptr;
  if (auto hint_value_iter_opt = findImageDataHint(&hints_iter)) {
    DBusMessageIter hint_value_iter = *hint_value_iter_opt;
    album_icon = parseIconFromDBusVariant(&hint_value_iter);
  }

  std::shared_ptr<const Metadata> new_metadata = std::make_shared<Metadata>(
      song, album, artist, PlaybackStatus::UNKNOWN, album_icon);
  atomic_store(&(provider->metadata), new_metadata);

  return DBUS_HANDLER_RESULT_HANDLED;
}

std::optional<DBusMessageIter> SpotifyNotificationsProvider::findImageDataHint(
    DBusMessageIter *hints_iter) {
  int current_type = dbus_message_iter_get_arg_type(hints_iter);
  while (current_type != DBUS_TYPE_INVALID) {
    DBusMessageIter hint_entry_iter;
    dbus_message_iter_recurse(hints_iter, &hint_entry_iter);
    char *hint_name_str = nullptr;
    dbus_message_iter_get_basic(&hint_entry_iter, &hint_name_str);
    std::string hint_name{hint_name_str};
    if (hint_name == kImageDataKey) {
      dbus_message_iter_next(&hint_entry_iter);
      DBusMessageIter hint_value_iter;
      dbus_message_iter_recurse(&hint_entry_iter, &hint_value_iter);
      return hint_value_iter;
    }
    dbus_message_iter_next(hints_iter);
    current_type = dbus_message_iter_get_arg_type(hints_iter);
  }
  return std::nullopt;
}

GdkPixbuf *SpotifyNotificationsProvider::parseIconFromDBusVariant(
    DBusMessageIter *variant_iter) {
  DBusMessageIter struct_iter;
  dbus_message_iter_recurse(variant_iter, &struct_iter);

  dbus_int32_t width = 0;
  dbus_message_iter_get_basic(&struct_iter, &width);
  dbus_message_iter_next(&struct_iter);
  dbus_int32_t height = 0;
  dbus_message_iter_get_basic(&struct_iter, &height);
  dbus_message_iter_next(&struct_iter);
  dbus_int32_t rowstride = 0;
  dbus_message_iter_get_basic(&struct_iter, &rowstride);
  dbus_message_iter_next(&struct_iter);
  dbus_bool_t has_alpha = FALSE;
  dbus_message_iter_get_basic(&struct_iter, &has_alpha);
  dbus_message_iter_next(&struct_iter);
  dbus_int32_t bits_per_sample = 0;
  dbus_message_iter_get_basic(&struct_iter, &bits_per_sample);
  dbus_message_iter_next(&struct_iter);
  dbus_int32_t n_channels = 0;
  dbus_message_iter_get_basic(&struct_iter, &n_channels);
  dbus_message_iter_next(&struct_iter);

  DBusMessageIter bytes_iter;
  dbus_message_iter_recurse(&struct_iter, &bytes_iter);
  const unsigned char *original_bytes = nullptr;
  int bytes_len = 0;
  dbus_message_iter_get_fixed_array(&bytes_iter, &original_bytes, &bytes_len);

  size_t pixelstride = (n_channels * bits_per_sample + 7) / 8;
  size_t len_expected = (height - 1) * rowstride + width * pixelstride;
  if (len_expected != bytes_len) {
    std::cout
        << "SpotifyNotificationsProvider: Album icon didn't pass sanity check"
        << std::endl;
    return nullptr;
  }

  GBytes *bytes = g_bytes_new(original_bytes, bytes_len);
  GdkPixbuf *pixbuf =
      gdk_pixbuf_new_from_bytes(bytes, GDK_COLORSPACE_RGB, has_alpha,
                                bits_per_sample, width, height, rowstride);

  if (pixbuf == nullptr) {
    std::cout << "SpotifyNotificationsProvider: Couldn't generate image from "
                 "album icon data"
              << std::endl;
    return nullptr;
  }

  return pixbuf;
}
