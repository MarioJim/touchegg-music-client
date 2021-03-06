#ifndef TOUCHEGG_MUSIC_CLIENT_PULSEAUDIO_ADAPTER_H
#define TOUCHEGG_MUSIC_CLIENT_PULSEAUDIO_ADAPTER_H

#include <pulse/pulseaudio.h>

/**
 * Interface to communicate with a PulseAudio server
 * Based on polybar's implementation
 * (https://github.com/polybar/polybar/blob/master/src/adapters/pulseaudio.cpp)
 */
class PulseAudioAdapter {
 public:
  PulseAudioAdapter();
  ~PulseAudioAdapter();

  PulseAudioAdapter(const PulseAudioAdapter& other) = delete;
  PulseAudioAdapter& operator=(const PulseAudioAdapter& other) = delete;
  PulseAudioAdapter(PulseAudioAdapter&& other) noexcept = delete;
  PulseAudioAdapter& operator=(PulseAudioAdapter&& other) noexcept = delete;

  void offset_volume(double delta_percentage);
  double get_volume();

 private:
  pa_threaded_mainloop* mainloop{nullptr};
  pa_context* context{nullptr};
  pa_cvolume sink_volume{};
  uint32_t sink_index;

  int success = 0;

  static constexpr const char* kDefaultSink = "@DEFAULT_SINK@";
};

#endif  // TOUCHEGG_MUSIC_CLIENT_PULSEAUDIO_ADAPTER_H
