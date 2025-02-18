#ifndef DONUTCPP_RENDERER_H_
#define DONUTCPP_RENDERER_H_

#include <vector>

class Renderer {
 public:
  Renderer();

  void Start();

 private:
  void Clear();
  void DrawBuffer();

  void MoveCursorTo(int x, int y);

  std::vector<char> buffer_;
  int width_;
  int height_;
};

#endif  // DONUTCPP_RENDERER_H_
