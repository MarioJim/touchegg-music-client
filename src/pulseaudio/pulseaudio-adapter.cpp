#include "pulseaudio/pulseaudio-adapter.h"

#include <pulse/pulseaudio.h>

#include <algorithm>
#include <iostream>

PulseAudioAdapter::PulseAudioAdapter() : mainloop(pa_threaded_mainloop_new()) {
  if (mainloop == nullptr) {
    throw std::runtime_error("Could not create pulseaudio threaded mainloop.");
  }

  pa_threaded_mainloop_lock(mainloop);

  context = pa_context_new(pa_threaded_mainloop_get_api(mainloop),
                           "touchegg_music_client");
  if (context == nullptr) {
    pa_threaded_mainloop_unlock(mainloop);
    pa_threaded_mainloop_free(mainloop);
    throw std::runtime_error("Could not create pulseaudio context.");
  }

  auto context_notify_callback = [](pa_context *c, void *userdata) {
    auto *adapter = static_cast<PulseAudioAdapter *>(userdata);
    switch (pa_context_get_state(c)) {
      case PA_CONTEXT_READY:
      case PA_CONTEXT_TERMINATED:
      case PA_CONTEXT_FAILED:
        pa_threaded_mainloop_signal(adapter->mainloop, 0);
        break;
      case PA_CONTEXT_UNCONNECTED:
      case PA_CONTEXT_CONNECTING:
      case PA_CONTEXT_AUTHORIZING:
      case PA_CONTEXT_SETTING_NAME:
        break;
    }
  };
  pa_context_set_state_callback(context, context_notify_callback, this);

  if (pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr) < 0) {
    pa_context_disconnect(context);
    pa_context_unref(context);
    pa_threaded_mainloop_unlock(mainloop);
    pa_threaded_mainloop_free(mainloop);
    throw std::runtime_error("Could not connect pulseaudio context.");
  }

  if (pa_threaded_mainloop_start(mainloop) < 0) {
    pa_context_disconnect(context);
    pa_context_unref(context);
    pa_threaded_mainloop_unlock(mainloop);
    pa_threaded_mainloop_free(mainloop);
    throw std::runtime_error("Could not start pulseaudio mainloop.");
  }

  pa_threaded_mainloop_wait(mainloop);
  if (pa_context_get_state(context) != PA_CONTEXT_READY) {
    pa_threaded_mainloop_unlock(mainloop);
    pa_threaded_mainloop_stop(mainloop);
    pa_context_disconnect(context);
    pa_context_unref(context);
    pa_threaded_mainloop_free(mainloop);
    throw std::runtime_error("Could not connect to pulseaudio server.");
  }

  auto sink_info_callback = [](pa_context * /*context*/,
                               const pa_sink_info *info, int eol,
                               void *userdata) {
    auto *adapter = static_cast<PulseAudioAdapter *>(userdata);
    if (eol == 0 && info != nullptr) {
      adapter->sink_index = info->index;
      adapter->sink_volume = info->volume;
    }
    pa_threaded_mainloop_signal(adapter->mainloop, 0);
  };
  pa_operation *op = pa_context_get_sink_info_by_name(context, kDefaultSink,
                                                      sink_info_callback, this);
  while (pa_operation_get_state(op) != PA_OPERATION_DONE) {
    pa_threaded_mainloop_wait(mainloop);
  }
  pa_operation_unref(op);

  pa_threaded_mainloop_unlock(mainloop);
}

PulseAudioAdapter::~PulseAudioAdapter() {
  pa_threaded_mainloop_stop(mainloop);
  pa_context_disconnect(context);
  pa_context_unref(context);
  pa_threaded_mainloop_free(mainloop);
}

void PulseAudioAdapter::offset_volume(double delta_percentage) {
  pa_threaded_mainloop_lock(mainloop);

  pa_volume_t delta_value = abs(delta_percentage) * PA_VOLUME_NORM / 100.0F;
  pa_volume_t delta_volume = std::clamp(delta_value, 0U, PA_VOLUME_NORM);
  if (delta_percentage > 0) {
    pa_volume_t current = pa_cvolume_max(&sink_volume);
    if (current + delta_volume <= PA_VOLUME_UI_MAX) {
      pa_cvolume_inc(&sink_volume, delta_volume);
    } else if (current < PA_VOLUME_UI_MAX) {
      pa_cvolume_scale(&sink_volume, PA_VOLUME_UI_MAX);
    } else {
      std::cout << "pulseaudio: maximum volume reached" << std::endl;
    }
  } else {
    pa_cvolume_dec(&sink_volume, delta_volume);
  }

  auto success_callback = [](pa_context * /*context*/, int success_result,
                             void *userdata) {
    auto *adapter = static_cast<PulseAudioAdapter *>(userdata);
    adapter->success = success_result;
    pa_threaded_mainloop_signal(adapter->mainloop, 0);
  };
  pa_operation *op = pa_context_set_sink_volume_by_index(
      context, sink_index, &sink_volume, success_callback, this);
  while (pa_operation_get_state(op) != PA_OPERATION_DONE) {
    pa_threaded_mainloop_wait(mainloop);
  }
  pa_operation_unref(op);
  if (success == 0) {
    throw std::runtime_error("Failed to set sink volume.");
  }

  pa_threaded_mainloop_unlock(mainloop);
}

double PulseAudioAdapter::get_volume() {
  pa_volume_t volume = pa_cvolume_max(&sink_volume);
  return static_cast<double>(volume) * 100.0 / PA_VOLUME_NORM + 0.5;
}
