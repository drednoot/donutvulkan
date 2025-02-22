#ifndef DONUTCPP_CONFIG_H_
#define DONUTCPP_CONFIG_H_

#include "vec3.h"

namespace config {
inline const int kTargetFps = 24;
inline const int kSceneLiveTimeSeconds = 30;

inline const int kCubeSidePresicion = 50;
inline const double kCubeSideSize = 0.5;

inline const char kLightLevles[] = ".,-~:;=!*#$@";
inline const int kLightLevelCount = sizeof(kLightLevles) / sizeof(char) - 1;
inline const quat::Vec3 kLightPoint = quat::Vec3({0.0, 0.0, -2.0}).Normalized();
}  // namespace config

#endif  // DONUTCPP_CONFIG_H_
