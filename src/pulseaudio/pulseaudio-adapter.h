#ifndef TOUCHEGG_MUSIC_CLIENT_PULSEAUDIO_ADAPTER_H
#define TOUCHEGG_MUSIC_CLIENT_PULSEAUDIO_ADAPTER_H

#include <pulse/pulseaudio.h>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

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
  static void context_callback(pa_context* ctx, void* userdata);
  void handle_context(pa_context* ctx);

  static void sink_info_callback(pa_context* ctx, const pa_sink_info* info,
                                 int eol, void* userdata);
  void handle_sink_info(const pa_sink_info* info, int eol);

  static void event_callback(pa_context* ctx,
                             pa_subscription_event_type_t event_type,
                             uint32_t event_sink_index, void* userdata);
  void handle_event(pa_subscription_event_type_t event_type,
                    uint32_t event_sink_index);

  static void success_callback(pa_context* ctx, int cb_success, void* userdata);

  pa_threaded_mainloop* mainloop{nullptr};
  pa_context* context{nullptr};
  pa_cvolume sink_volume{};
  uint32_t sink_index{};

  std::mutex sink_mutex{};
  std::condition_variable pending_sink_op_cv{};
  enum SinkOperation {
    NONE,
    RESET_SINK,
    UPDATE_SINK_PROPS,
  };
  SinkOperation pending_sink_op{NONE};

  int success = 0;

  static constexpr const char* kDefaultSink = "@DEFAULT_SINK@";
};

#endif  // TOUCHEGG_MUSIC_CLIENT_PULSEAUDIO_ADAPTER_H
