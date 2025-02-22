#ifndef DONUTCPP_APP_RENDERER_H_
#define DONUTCPP_APP_RENDERER_H_

#include "abstract_renderer.h"
#include "cube.h"

class Renderer : public core::AbstractRenderer {
 public:
  Renderer();

 protected:
  virtual void Render(double delta);

 private:
  Cube cube_;
  double angle_;
};

#endif  // DONUTCPP_APP_RENDERER_H_
