#ifndef DONUTCPP_APP_CUBE_H_
#define DONUTCPP_APP_CUBE_H_

#include "core/object.h"

class Cube : public core::Object {
 public:
  Cube(double side_size, int precision);

 private:
  double side_size_;
  int precision_;
};

#endif  // DONUTCPP_APP_CUBE_H_
