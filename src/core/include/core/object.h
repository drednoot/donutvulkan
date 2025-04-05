#ifndef DONUTCPP_CORE_OBJECT_H_
#define DONUTCPP_CORE_OBJECT_H_

#include <vector>

#include "point_info.h"

namespace core {

class Object {
 public:
  inline std::vector<PointInfo> const& Points() const { return points_; }

 protected:
  Object() {}

  std::vector<PointInfo> points_;
};

}  // namespace core

#endif  // DONUTCPP_CORE_OBJECT_H_
