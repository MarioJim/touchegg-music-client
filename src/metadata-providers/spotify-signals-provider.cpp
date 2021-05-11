#include "metadata-providers/spotify-signals-provider.h"

#include <string>
#include <utility>

SpotifySignalsProvider::SpotifySignalsProvider() {
  Gio::init();

  try {
    dbus_connection =
        Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
  } catch (const Glib::Error &ex) {
    std::cout << "Error creating DBus connection: " << ex.what() << std::endl;
    return;
  }

  Gio::DBus::Connection::SlotSignal slot =
      sigc::mem_fun(this, &SpotifySignalsProvider::onPropertiesChanged);
  dbus_connection->signal_subscribe(slot, "", "org.freedesktop.DBus.Properties",
                                    "PropertiesChanged",
                                    "/org/mpris/MediaPlayer2", "");
}

SpotifySignalsProvider::~SpotifySignalsProvider() { dbus_connection.reset(); }

std::shared_ptr<const Metadata> SpotifySignalsProvider::getMetadata() {
  return atomic_load(&metadata);
}

void SpotifySignalsProvider::onPropertiesChanged(
    const Glib::RefPtr<Gio::DBus::Connection> & /*connection*/,
    const Glib::ustring & /*sender_name*/,
    const Glib::ustring & /*object_path*/,
    const Glib::ustring & /*interface_name*/,
    const Glib::ustring & /*signal_name*/,
    const Glib::VariantContainerBase &parameters) {
  Glib::VariantBase changed_properties_base;
  parameters.get_child(changed_properties_base, 1);

  VariantDict changed_properties = variantToDict(changed_properties_base);

  Glib::VariantBase metadata_dict_base;
  if (changed_properties.find("Metadata") != changed_properties.end()) {
    metadata_dict_base = changed_properties.at("Metadata");
  } else {
    this->metadata.reset();
    return;
  }

  VariantDict metadata_dict = variantToDict(metadata_dict_base);

  Glib::VariantBase playback_status_base;
  if (changed_properties.find("PlaybackStatus") != changed_properties.end()) {
    playback_status_base = changed_properties.at("PlaybackStatus");
  } else {
    this->metadata.reset();
    return;
  }

  std::string playback_status_str = variantToString(playback_status_base);

  std::shared_ptr<const Metadata> new_metadata =
      SpotifySignalsProvider::parseMetadata(metadata_dict, playback_status_str);

  atomic_store(&(this->metadata), new_metadata);
}

std::shared_ptr<const Metadata> SpotifySignalsProvider::parseMetadata(
    const VariantDict &metadata_dict, const std::string &playback_status_str) {
  std::string song;
  if (metadata_dict.find("xesam:title") != metadata_dict.end()) {
    song = variantToString(metadata_dict.at("xesam:title"));
  }

  std::string album;
  if (metadata_dict.find("xesam:album") != metadata_dict.end()) {
    album = variantToString(metadata_dict.at("xesam:album"));
  }

  std::string artist;
  if (metadata_dict.find("xesam:artist") != metadata_dict.end()) {
    auto artists_vec =
        Glib::VariantBase::cast_dynamic<Glib::VariantContainerBase>(
            metadata_dict.at("xesam:artist"));
    artist = variantToString(artists_vec.get_child());
  }

  PlaybackStatus playback_status =
      playbackStatusFromString(playback_status_str);

  return std::make_shared<const Metadata>(std::move(song), std::move(album),
                                          std::move(artist), playback_status,
                                          nullptr);
}

std::string SpotifySignalsProvider::variantToString(
    const Glib::VariantBase &variant) {
  return Glib::VariantBase::cast_dynamic<Glib::Variant<std::string>>(variant)
      .get();
}

VariantDict SpotifySignalsProvider::variantToDict(
    const Glib::VariantBase &variant) {
  return Glib::VariantBase::cast_dynamic<Glib::Variant<VariantDict>>(variant)
      .get();
}
