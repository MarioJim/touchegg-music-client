#ifndef TOUCHEGG_MUSIC_CLIENT_ACTION_H
#define TOUCHEGG_MUSIC_CLIENT_ACTION_H

#include "gesture/gesture.h"

class Action {
 public:
  Action() = default;
  virtual ~Action() = default;

  Action(const Action& other) = delete;
  Action& operator=(const Action& other) = delete;
  Action(Action&& other) noexcept = delete;
  Action& operator=(Action&& other) noexcept = delete;

  virtual void onGestureBegin(const Gesture& gesture) = 0;
  virtual void onGestureUpdate(const Gesture& gesture) = 0;
  virtual void onGestureEnd(const Gesture& gesture) = 0;
};

#endif  // TOUCHEGG_MUSIC_CLIENT_ACTION_H
