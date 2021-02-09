#ifndef TOUCHEGG_MUSIC_CLIENT_CUSTOM_ACTION_H
#define TOUCHEGG_MUSIC_CLIENT_CUSTOM_ACTION_H

#include "gesture/gesture.h"

class CustomAction {
 public:
  virtual ~CustomAction() = default;
  virtual void onGestureBegin(const Gesture &gesture) = 0;
  virtual void onGestureUpdate(const Gesture &gesture) = 0;
  virtual void onGestureEnd(const Gesture &gesture) = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_CUSTOM_ACTION_H
