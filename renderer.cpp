#include "renderer.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "config.h"

Renderer::Renderer()
    : width_(0),
      height_(0),
      screen_ratio_(0.0),
      char_ratio_(0.5),
      target_ns_(std::chrono::nanoseconds(std::chrono::seconds(1)) /
                 config::kTargetFps) {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  width_ = w.ws_col;
  height_ = w.ws_row;
  buffer_.resize(width_ * height_);

  screen_ratio_ = width_ / (double)height_;
}

void Renderer::Start() {
  Clear();
  int live_frames =
      config::kSceneLiveTimeSeconds * config::kTargetFps;  // 10 seconds

  std::chrono::nanoseconds delta = std::chrono::nanoseconds(0);
  time_since_start_ = std::chrono::system_clock::now().time_since_epoch();

  while (live_frames >= 0) {
    std::chrono::time_point before_render = std::chrono::system_clock::now();
    Render(delta);
    DrawBuffer();
    std::chrono::time_point after_render = std::chrono::system_clock::now();
    delta = after_render - before_render;

    if (delta < target_ns_) {
      std::this_thread::sleep_for(target_ns_ - delta);
    }

    --live_frames;
  }
}

void Renderer::Clear() {
  for (int i = 0; i < width_ * height_; ++i) {
    buffer_[i] = ' ';
  }
}

void Renderer::Render(std::chrono::nanoseconds delta) {
  const double r = 0.05;

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const double x_norm = ToAspect(x / (double)width_);
      const double y_norm = y / (double)height_;

      const double x0 = x_norm - 0.5;
      const double y0 = y_norm - 0.5;

      if (x0 * x0 + y0 * y0 <= r * r) {
        buffer_[Xy(x, y)] = '@';
      }
      if (x_norm >= 1) {
        buffer_[Xy(x, y)] = '>';
      }
      if (y_norm >= 1) {
        buffer_[Xy(x, y)] = 'v';
      }
    }
  }
}

void Renderer::DrawBuffer() const {
  MoveCursorTo(0, 0);
  std::cout.write(buffer_.data(), buffer_.size());
  std::cout.flush();
}

void Renderer::MoveCursorTo(int x, int y) const {
  printf("\033[%d;%dH", y, x);
}
