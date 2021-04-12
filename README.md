![Screenshot](./.github/images/screenshot.png)

# touchegg-music-client

![Lines of code](https://tokei.rs/b1/github/MarioJim/touchegg-music-client?category=code)
![Continuous Integration](https://github.com/MarioJim/touchegg-music-client/workflows/Continuous%20Integration/badge.svg)

Widget for music and volume control, connected to [touchégg](https://github.com/JoseExposito/touchegg) 's daemon.

## How does it work?

The client connects to touchégg's daemon, and on every gesture update it asks every provider until one responds with a
metadata struct, and then displays the information through different windows.

### Metadata providers

The information displayed on the windows is sourced from different providers, there's currently only two providers:

#### Spotify Notifications Provider

It works by monitoring the notifications bus and parsing only the Spotify notifications. It supports showing the album
icon but it currently doesn't support the playback status.

#### Spotify Polling Provider

It works by polling Spotify's `org.mpris.MediaPlayer2.Player` DBus interface every 500ms.

### Windows

The program can show three different windows:

#### Volume window

This window is always displayed, it shows the current volume in the default PulseAudio sink.

#### Metadata window

If one of the providers returns a valid metadata struct, this window displays the song and artist's name, and if the
provider supports it an icon representing the playback status also is displayed.

#### Album icon window

If the metadata struct has a valid album icon, this window displays it.

## Dependencies

- Have [touchégg](https://github.com/JoseExposito/touchegg) installed and running
- libpulse (`libpulse` in Arch-based distros, `libpulse-dev` in Debian-based distros)

## Installation

You can find the package in the AUR
as [touchegg-music-client](https://aur.archlinux.org/packages/touchegg-music-client/)

```shell
$ paru -S touchegg-music-client
```

## Todo

- [x] Display album icon
- [ ] SpotifyNotificationsProvider listening to Spotify's `PropertiesChanged` signal
- [ ] SpotifyPollingProvider: configurable timer for polling
- [ ] Configurable number of fingers/gesture direction
- [ ] Configurable windows
- [ ] Configurable providers priority
- [ ] Configurable PulseAudio sink
- [ ] Add a configuration file
- [ ] Add new providers (chromium)