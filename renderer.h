#ifndef DONUTCPP_RENDERER_H_
#define DONUTCPP_RENDERER_H_

#include <chrono>
#include <vector>

class Renderer {
 public:
  Renderer();

  void Start();

 private:
  void Clear();
  void Render(std::chrono::nanoseconds delta);
  void DrawBuffer() const;

  void MoveCursorTo(int x, int y) const;

  inline int Xy(int x, int y) {
    return y * width_ + x;
  }  // converts x and y to buffer index
  inline int Right() { return width_ - 1; }  // returns rightmost "pixel"
  inline int Bot() { return height_ - 1; }   // returns bottommost "pixel"

  std::vector<char> buffer_;
  int width_;
  int height_;

  std::chrono::nanoseconds target_ns_;
  std::chrono::nanoseconds time_since_start_;
};

#endif  // DONUTCPP_RENDERER_H_
