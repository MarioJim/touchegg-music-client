#include "windows/windows-config.h"

#include <algorithm>

double WindowsConfig::indicatorY(const Rectangle &monitor,
                                 double percentage) const {
  return monitor.y + 190 - indicatorHeight(percentage);
}

double WindowsConfig::indicatorHeight(double percentage) const {
  return 0.9 * std::clamp(percentage, 0.0, 100.0);
}

double WindowsConfig::indicatorBackgroundX(const Rectangle &monitor) const {
  return monitor.x + 64;
}

double WindowsConfig::backgroundY(const Rectangle &monitor) const {
  return indicatorY(monitor, 100) - kIndicatorBackgroundTopMargin;
}

double WindowsConfig::indicatorBackgroundWidth() const {
  return kIndicatorWidth + 2 * kIndicatorBackgroundHorizMargin;
}

double WindowsConfig::backgroundHeight() const {
  return indicatorHeight(100) + kIndicatorBackgroundTopMargin +
         kIndicatorBackgroundBottomMargin;
}
