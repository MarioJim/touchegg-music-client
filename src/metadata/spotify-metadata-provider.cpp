#include "spotify-metadata-provider.h"

#include <iostream>

SpotifyMetadataProvider::SpotifyMetadataProvider() {
  if (isSpotifyDBusConnected()) {
    initSpotifyProxy();
  }
}

SpotifyMetadataProvider::~SpotifyMetadataProvider() {
  if (spotify_proxy != nullptr) {
    g_object_unref(spotify_proxy);
  }
}

std::unique_ptr<Metadata> SpotifyMetadataProvider::getMetadata() {
  if (spotify_proxy == nullptr) {
    bool was_spotify_proxy_created = initSpotifyProxy();
    if (!was_spotify_proxy_created) {
      return nullptr;
    }
  }

  GError *tmp_error = nullptr;
  GVariant *reply = g_dbus_proxy_call_sync(
      spotify_proxy, "Get",
      g_variant_new("(ss)", "org.mpris.MediaPlayer2.Player", "Metadata"),
      G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &tmp_error);
  if (tmp_error != nullptr) {
    std::cout << "Error listing Spotify's metadata: " << tmp_error->message
              << std::endl;
    g_object_unref(spotify_proxy);
    spotify_proxy = nullptr;
    return nullptr;
  }

  GVariant *reply_child = g_variant_get_child_value(reply, 0);
  GVariant *metadata_dict = g_variant_get_child_value(reply_child, 0);

  g_variant_unref(reply);
  g_variant_unref(reply_child);

  return parseMetadataFromGVariant(metadata_dict);
}

bool SpotifyMetadataProvider::initSpotifyProxy() {
  GError *tmp_error = nullptr;
  spotify_proxy = g_dbus_proxy_new_for_bus_sync(
      G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE, nullptr,
      "org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2",
      "org.freedesktop.DBus.Properties", nullptr, &tmp_error);
  if (tmp_error != nullptr) {
    std::cout << "Error creating Spotify DBus proxy: " << tmp_error->message
              << std::endl;
    return false;
  }
  return true;
}

bool SpotifyMetadataProvider::isSpotifyDBusConnected() {
  GError *tmp_error = nullptr;
  GDBusProxy *dbus_proxy = g_dbus_proxy_new_for_bus_sync(
      G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE, nullptr,
      "org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus",
      nullptr, &tmp_error);
  if (tmp_error != nullptr) {
    std::cout << "Error creating DBus proxy: " << tmp_error->message
              << std::endl;
    return false;
  }

  GVariant *reply =
      g_dbus_proxy_call_sync(dbus_proxy, "ListNames", nullptr,
                             G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &tmp_error);
  if (tmp_error != nullptr) {
    std::cout << "Error listing DBus buses: " << tmp_error->message
              << std::endl;
    g_object_unref(dbus_proxy);
    return false;
  }

  bool spotify_bus_found = false;
  GVariant *reply_child = g_variant_get_child_value(reply, 0);
  gsize reply_count = 0;
  const gchar **names = g_variant_get_strv(reply_child, &reply_count);
  for (gsize i = 0; i < reply_count; i += 1) {
    if (g_strcmp0(names[i], kSpotifyDBusName) == 0) {
      spotify_bus_found = true;
    }
  }

  g_object_unref(dbus_proxy);
  g_variant_unref(reply);
  g_variant_unref(reply_child);
  g_free(names);

  return spotify_bus_found;
}

std::unique_ptr<Metadata> SpotifyMetadataProvider::parseMetadataFromGVariant(
    GVariant *metadata_dict) {
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

  g_variant_unref(metadata_dict);

  return std::make_unique<Metadata>(song, album, artist);
}