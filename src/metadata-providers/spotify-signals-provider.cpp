#include "spotify-signals-provider.h"

SpotifySignalsProvider::SpotifySignalsProvider() {
  initSpotifyProxy();
  // Should I run g_main_loop_run?
  // It's already executed in DaemonClient::run()
}

SpotifySignalsProvider::~SpotifySignalsProvider() {
  g_object_unref(dbus_connection);
}

std::shared_ptr<const Metadata> SpotifySignalsProvider::getMetadata() {
  return atomic_load(&metadata);
}

bool SpotifySignalsProvider::initSpotifyProxy() {
  GError *tmp_error = nullptr;

  dbus_connection = g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, &tmp_error);
  if (tmp_error != nullptr) {
    std::cout << "Error creating DBus connection: " << tmp_error->message
              << std::endl;
    return false;
  }

  g_dbus_connection_signal_subscribe(
      dbus_connection, nullptr, "org.freedesktop.DBus.Properties",
      "PropertiesChanged", "/org/mpris/MediaPlayer2", nullptr,
      G_DBUS_SIGNAL_FLAGS_NONE, SpotifySignalsProvider::onPropertiesChanged,
      this, nullptr);

  return true;
}

void SpotifySignalsProvider::onPropertiesChanged(
    GDBusConnection * /*connection*/, const gchar * /*sender_name*/,
    const gchar * /*object_path*/, const gchar * /*interface_name*/,
    const gchar * /*signal_name*/, GVariant *parameters, gpointer user_data) {
  auto *provider = static_cast<SpotifySignalsProvider *>(user_data);

  const gchar *interface_name_for_signal = nullptr;
  GVariant *changed_properties = nullptr;
  gchar **invalidated_properties = nullptr;
  g_variant_get(parameters, "(&s@a{sv}^a&s)", &interface_name_for_signal,
                &changed_properties, &invalidated_properties);
  g_free(invalidated_properties);

  GVariantIter iter;
  g_variant_iter_init(&iter, changed_properties);
  gchar *key = nullptr;
  GVariant *value = nullptr;
  GVariant *metadata_dict = nullptr;
  GVariant *playback_status_var = nullptr;
  while (g_variant_iter_next(&iter, "{sv}", &key, &value) != FALSE) {
    if (g_strcmp0(key, "Metadata") == 0) {
      metadata_dict = value;
    } else if (g_strcmp0(key, "PlaybackStatus") == 0) {
      playback_status_var = value;
    } else {
      g_variant_unref(value);
    }
    g_free(key);
  }
  g_clear_pointer(&changed_properties, g_variant_unref);

  if (metadata_dict == nullptr || playback_status_var == nullptr) {
    return;
  }
  std::shared_ptr<const Metadata> new_metadata =
      SpotifySignalsProvider::metadataFromGVariant(metadata_dict,
                                                   playback_status_var);
  atomic_store(&(provider->metadata), new_metadata);

  g_variant_unref(metadata_dict);
  g_variant_unref(playback_status_var);
}

std::shared_ptr<const Metadata> SpotifySignalsProvider::metadataFromGVariant(
    GVariant *metadata_dict, GVariant *playback_status_variant) {
  GVariant *song_variant = g_variant_lookup_value(metadata_dict, "xesam:title",
                                                  G_VARIANT_TYPE_STRING);
  std::string song;
  if (song_variant != nullptr) {
    song = g_variant_get_string(song_variant, nullptr);
    g_variant_unref(song_variant);
  }

  GVariant *album_variant = g_variant_lookup_value(metadata_dict, "xesam:album",
                                                   G_VARIANT_TYPE_STRING);
  std::string album;
  if (album_variant != nullptr) {
    album = g_variant_get_string(album_variant, nullptr);
    g_variant_unref(album_variant);
  }

  if (song.empty() && album.empty()) {
    return nullptr;
  }

  GVariant *artist_variant = g_variant_lookup_value(
      metadata_dict, "xesam:artist", G_VARIANT_TYPE_STRING_ARRAY);
  std::string artist;
  if (artist_variant != nullptr) {
    gsize artists_size = 0;
    const gchar **artists = g_variant_get_strv(artist_variant, &artists_size);
    if (artists_size >= 1) {
      artist = artists[0];
    }
    g_free(artists);
    g_variant_unref(artist_variant);
  }

  std::string playback_status_str =
      g_variant_get_string(playback_status_variant, nullptr);
  PlaybackStatus playback_status =
      playbackStatusFromString(playback_status_str);

  return std::make_shared<const Metadata>(song, album, artist, playback_status,
                                          nullptr);
}
