#ifndef DONUTCPP_ABSTRACT_RENDERER_H_
#define DONUTCPP_ABSTRACT_RENDERER_H_

#include <chrono>
#include <vector>

#include "vec3.h"

class AbstractRenderer {
 public:
  AbstractRenderer(int target_fps = 24, int scene_live_time = 30);

  void Start();

 protected:
  virtual void Render(double delta) = 0;

  void Clear();
  // puts a char `sym` on the screen at a point (x, y), doesn't check bounds
  inline void Put(int x, int y, char sym) { buffer_[Xy(x, y)] = sym; }
  /**
   * puts a char `sym` on the screen at a point `point`, where 0 <= x, y, z < 1
   * checks bounds, checks depth, (probably) slower than Put(int x, int y, char
   * sym)
   */
  void Put(quat::Vec3 point, char sym);

  int Width() const { return width_; };
  int Height() const { return height_; };
  double Ratio() const { return screen_ratio_ * char_ratio_; };

  // converts x and y to buffer index
  inline int Xy(int x, int y) const { return y * width_ + x; }
  // returns rightmost "pixel"
  inline int Right() const { return width_ - 1; }
  // returns bottommost "pixel"
  inline int Bot() const { return height_ - 1; }
  inline double ToAspect(double val) const { return val * Ratio(); }
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

 private:
  void DrawBuffer() const;

  void MoveCursorTo(int x, int y) const;

  std::vector<char> buffer_;
  std::vector<double> z_buffer_;
  int width_;
  int height_;
  double screen_ratio_;
  double char_ratio_;
  int target_fps_;
  int scene_live_time_;

  std::chrono::nanoseconds target_ns_;
  std::chrono::nanoseconds start_time_;
};

#endif  // DONUTCPP_ABSTRACT_RENDERER_H_
