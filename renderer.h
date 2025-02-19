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
  void Render(double delta);
  void DrawBuffer() const;

  void MoveCursorTo(int x, int y) const;

  inline int Xy(int x, int y) const {
    return y * width_ + x;
  }  // converts x and y to buffer index
  inline int Right() const { return width_ - 1; }  // returns rightmost "pixel"
  inline int Bot() const { return height_ - 1; }   // returns bottommost "pixel"
  inline double ToAspect(double val) const {
    return val * screen_ratio_ * char_ratio_;
  }
  inline double NsToSeconds(std::chrono::nanoseconds ns) const {
    const long count =
        std::chrono::duration_cast<std::chrono::milliseconds, long>(ns).count();
    return count / 1000.0;
  }
  inline double TimeLived() const {
    std::chrono::nanoseconds now =
        std::chrono::system_clock::now().time_since_epoch();
    std::chrono::nanoseconds from_start = now - start_time_;
    return NsToSeconds(from_start);
  }

  std::vector<char> buffer_;
  int width_;
  int height_;
  double screen_ratio_;
  double char_ratio_;

  std::chrono::nanoseconds target_ns_;
  std::chrono::nanoseconds start_time_;
};

#endif  // DONUTCPP_RENDERER_H_
