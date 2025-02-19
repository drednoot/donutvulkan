#include "renderer.h"

#include <cmath>
#include <string>

#include "abstract_renderer.h"

Renderer::Renderer() : AbstractRenderer() {}

void Renderer::Render(double delta) {
  const double r = 0.05;
  const double time_lived = TimeLived();
  static double coord = 0.0;
  coord += 1 * delta;

  for (int y = 0; y < Height(); ++y) {
    for (int x = 0; x < Width(); ++x) {
      const double x_norm = ToAspect(x / (double)Width());
      const double y_norm = y / (double)Height();

      double x0 = x_norm - 0.5;
      double y0 = y_norm - 0.5;

      x0 = x0 + sin(coord) / 2.0;

      if (x0 * x0 + y0 * y0 <= r * r) {
        Buffer()[Xy(x, y)] = '@';
      }
      if (x_norm >= 1) {
        Buffer()[Xy(x, y)] = '>';
      }
      if (y_norm >= 1) {
        Buffer()[Xy(x, y)] = 'v';
      }
    }
  }

  std::string delta_imm = std::to_string(delta);
  for (int i = 0, size = delta_imm.size(); i < size; ++i) {
    Buffer()[Xy(i, Bot() - 2)] = delta_imm[i];
  }
  std::string sin_imm = std::to_string(sin(time_lived * delta * 10.0) / 2.0);
  for (int i = 0, size = sin_imm.size(); i < size; ++i) {
    Buffer()[Xy(i, Bot() - 1)] = sin_imm[i];
  }
  std::string dfs = std::to_string(time_lived);
  for (int i = 0, size = dfs.size(); i < size; ++i) {
    Buffer()[Xy(i, Bot())] = dfs[i];
  }
}
