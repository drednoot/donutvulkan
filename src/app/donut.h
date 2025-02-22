#ifndef DONUTCPP_APP_DONUT_H_
#define DONUTCPP_APP_DONUT_H_

#include "object.h"

/**
 *  Initializes Donut points.
 *  @param r1 major radius
 *  @param r2 minor radius
 *  @param precision amount of points along the minor and circles along the
 * major radiuses
 */
class Donut : public core::Object {
 public:
  Donut(double r1, double r2, int precision);
};

#endif  // DONUTCPP_APP_DONUT_H_
