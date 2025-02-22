#ifndef DONUTCPP_RENDERER_H_
#define DONUTCPP_RENDERER_H_

#include <array>

#include "abstract_renderer.h"
#include "config.h"
#include "vec3.h"

class Renderer : public core::AbstractRenderer {
 public:
  Renderer();

 protected:
  virtual void Render(double delta);

 private:
  struct PointInfo {
    core::Vec3 p;
    core::Vec3 normal;
  };
  std::array<PointInfo,
             config::kCubeSidePresicion * config::kCubeSidePresicion * 6>
      cube_points_;
  double angle_;
};

#endif  // DONUTCPP_RENDERER_H_
