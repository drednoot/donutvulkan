#ifndef DONUTCPP_APP_CONFIG_H_
#define DONUTCPP_APP_CONFIG_H_

#include "vec3.h"

namespace config {
inline const int kTargetFps = 60;
inline const int kSceneLiveTimeSeconds = 30;

inline const int kCubeSidePresicion = 100;
inline const double kCubeSideSize = 0.5;

inline const char kLightLevles[] = ".,-_:;=+*#%@";
inline const int kLightLevelCount = sizeof(kLightLevles) / sizeof(char) - 1;
inline const core::Vec3 kLightPoint = core::Vec3({0.0, 0.0, 2.0}).Normalized();
}  // namespace config

#endif  // DONUTCPP_APP_CONFIG_H_
