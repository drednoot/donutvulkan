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
      target_ns_(std::chrono::nanoseconds(std::chrono::seconds(1)) /
                 config::kTargetFps) {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  width_ = w.ws_col;
  height_ = w.ws_row;
  buffer_.resize(width_ * height_);
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
  std::string delta_num = std::to_string(delta.count());

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      double x_norm = x / (double)width_;
      double y_norm = y / (double)height_;

      if (x_norm * x_norm + y_norm * y_norm <= 1.0) {
        buffer_[Xy(x, y)] = '@';
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
