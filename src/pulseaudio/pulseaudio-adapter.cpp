#include "pulseaudio/pulseaudio-adapter.h"

#include <pulse/pulseaudio.h>

#include <algorithm>
#include <iostream>
#include <thread>

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

  pa_context_set_state_callback(context, context_callback, this);

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

  // Get the default sink's name
  pa_operation *operation = pa_context_get_sink_info_by_name(
      context, kDefaultSink, sink_info_callback, this);

  while (pa_operation_get_state(operation) != PA_OPERATION_DONE) {
    pa_threaded_mainloop_wait(mainloop);
  }
  pa_operation_unref(operation);

  // Subscribe to server/sink changes
  auto event_types = static_cast<pa_subscription_mask_t>(
      PA_SUBSCRIPTION_MASK_SINK | PA_SUBSCRIPTION_MASK_SERVER);
  operation =
      pa_context_subscribe(context, event_types, success_callback, this);
  while (pa_operation_get_state(operation) != PA_OPERATION_DONE) {
    pa_threaded_mainloop_wait(mainloop);
  }
  pa_operation_unref(operation);
  if (success == 0) {
    throw std::runtime_error("Failed to subscribe to sink.");
  }
  pa_context_set_subscribe_callback(context, event_callback, this);

  pa_threaded_mainloop_unlock(mainloop);

  std::thread update_sink_index_thread([this]() {
    while (true) {
      std::unique_lock lock(sink_mutex);
      pending_sink_op_cv.wait(lock, [this] { return pending_sink_op != NONE; });
      SinkOperation sink_op = pending_sink_op;
      pending_sink_op = NONE;
      lock.unlock();

      pa_threaded_mainloop_lock(mainloop);
      pa_operation *operation =
          (sink_op == UPDATE_SINK_PROPS)
              ? pa_context_get_sink_info_by_index(context, sink_index,
                                                  sink_info_callback, this)
              : pa_context_get_sink_info_by_name(context, kDefaultSink,
                                                 sink_info_callback, this);

      while (pa_operation_get_state(operation) != PA_OPERATION_DONE) {
        pa_threaded_mainloop_wait(mainloop);
      }
      pa_operation_unref(operation);
      pa_threaded_mainloop_unlock(mainloop);
    }
  });
  update_sink_index_thread.detach();
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

  pa_operation *operation = pa_context_set_sink_volume_by_index(
      context, sink_index, &sink_volume, success_callback, this);
  while (pa_operation_get_state(operation) != PA_OPERATION_DONE) {
    pa_threaded_mainloop_wait(mainloop);
  }
  pa_operation_unref(operation);
  if (success == 0) {
    throw std::runtime_error("Failed to set sink volume.");
  }

  pa_threaded_mainloop_unlock(mainloop);
}

double PulseAudioAdapter::get_volume() {
  pa_volume_t volume = pa_cvolume_max(&sink_volume);
  return static_cast<double>(volume) * 100.0 / PA_VOLUME_NORM + 0.5;
}

void PulseAudioAdapter::context_callback(pa_context *ctx, void *userdata) {
  static_cast<PulseAudioAdapter *>(userdata)->handle_context(ctx);
}

void PulseAudioAdapter::handle_context(pa_context *ctx) {
  switch (pa_context_get_state(ctx)) {
    case PA_CONTEXT_READY:
    case PA_CONTEXT_TERMINATED:
    case PA_CONTEXT_FAILED:
      pa_threaded_mainloop_signal(mainloop, 0);
      break;
    case PA_CONTEXT_UNCONNECTED:
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
      break;
  }
}

void PulseAudioAdapter::sink_info_callback(pa_context * /*ctx*/,
                                           const pa_sink_info *info, int eol,
                                           void *userdata) {
  static_cast<PulseAudioAdapter *>(userdata)->handle_sink_info(info, eol);
}

void PulseAudioAdapter::handle_sink_info(const pa_sink_info *info, int eol) {
  if (eol == 0 && info != nullptr) {
    sink_index = info->index;
    sink_volume = info->volume;
  }

  pa_threaded_mainloop_signal(mainloop, 0);
}

void PulseAudioAdapter::event_callback(pa_context * /*ctx*/,
                                       pa_subscription_event_type_t event_type,
                                       uint32_t event_sink_index,
                                       void *userdata) {
  static_cast<PulseAudioAdapter *>(userdata)->handle_event(event_type,
                                                           event_sink_index);
}

void PulseAudioAdapter::handle_event(pa_subscription_event_type_t event_type,
                                     uint32_t event_sink_index) {
  auto event_facility = static_cast<pa_subscription_event_type_t>(
      event_type & PA_SUBSCRIPTION_EVENT_FACILITY_MASK);
  auto event_operation = static_cast<pa_subscription_event_type_t>(
      event_type & PA_SUBSCRIPTION_EVENT_TYPE_MASK);

  bool server_changed = event_facility == PA_SUBSCRIPTION_EVENT_SERVER &&
                        event_operation == PA_SUBSCRIPTION_EVENT_CHANGE;
  bool new_sink = event_facility == PA_SUBSCRIPTION_EVENT_SINK &&
                  event_operation == PA_SUBSCRIPTION_EVENT_NEW;
  bool current_sink_removed = event_facility == PA_SUBSCRIPTION_EVENT_SINK &&
                              event_operation == PA_SUBSCRIPTION_EVENT_REMOVE &&
                              event_sink_index == sink_index;
  bool current_sink_changed = event_facility == PA_SUBSCRIPTION_EVENT_SINK &&
                              event_operation == PA_SUBSCRIPTION_EVENT_CHANGE &&
                              event_sink_index == sink_index;

  pa_threaded_mainloop_signal(mainloop, 0);

  SinkOperation new_op = NONE;
  if (server_changed || new_sink || current_sink_removed) {
    new_op = RESET_SINK;
  } else if (current_sink_changed) {
    new_op = UPDATE_SINK_PROPS;
  }

  if (new_op != NONE) {
    {
      std::scoped_lock lock(sink_mutex);
      pending_sink_op = new_op;
    }
    pending_sink_op_cv.notify_one();
  }
}

void PulseAudioAdapter::success_callback(pa_context * /*ctx*/, int cb_success,
                                         void *userdata) {
  auto *adapter = static_cast<PulseAudioAdapter *>(userdata);
  adapter->success = cb_success;
  pa_threaded_mainloop_signal(adapter->mainloop, 0);
}
