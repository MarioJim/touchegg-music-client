#ifndef TOUCHEGG_MUSIC_CLIENT_ACTION_H
#define TOUCHEGG_MUSIC_CLIENT_ACTION_H

#include "gesture/gesture.h"

class Action {
 public:
  virtual ~Action() = default;
  virtual void onGestureBegin(const Gesture &gesture) = 0;
  virtual void onGestureUpdate(const Gesture &gesture) = 0;
  virtual void onGestureEnd(const Gesture &gesture) = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_ACTION_H
