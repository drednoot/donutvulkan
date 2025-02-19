#ifndef DONUTCPP_RENDERER_H_
#define DONUTCPP_RENDERER_H_

#include "abstract_renderer.h"

class Renderer : public AbstractRenderer {
 public:
  Renderer();

 protected:
  virtual void Render(double delta);

 private:
  double coord_;
};

#endif  // DONUTCPP_RENDERER_H_
