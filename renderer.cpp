#include "renderer.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <chrono>
#include <cmath>
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
  int live_frames =
      config::kSceneLiveTimeSeconds * config::kTargetFps;  // 10 seconds

  std::chrono::nanoseconds frame_time = target_ns_;
  double delta = NsToSeconds(target_ns_);
  start_time_ = std::chrono::system_clock::now().time_since_epoch();

  while (live_frames >= 0) {
    std::chrono::time_point before_render = std::chrono::system_clock::now();
    Clear();
    Render(delta);
    DrawBuffer();
    std::chrono::time_point after_render = std::chrono::system_clock::now();
    frame_time = after_render - before_render;
    delta = frame_time > target_ns_ ? NsToSeconds(frame_time)
                                    : NsToSeconds(target_ns_);

    if (frame_time < target_ns_) {
      std::this_thread::sleep_for(target_ns_ - frame_time);
    }

    --live_frames;
  }
}

void Renderer::Clear() {
  for (int i = 0; i < width_ * height_; ++i) {
    buffer_[i] = ' ';
  }
}

void Renderer::Render(double delta) {
  const double r = 0.05;
  const double time_lived = TimeLived();
  static double coord = 0.0;
  coord += 1 * delta;

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const double x_norm = ToAspect(x / (double)width_);
      const double y_norm = y / (double)height_;

      double x0 = x_norm - 0.5;
      double y0 = y_norm - 0.5;

      x0 = x0 + sin(coord) / 2.0;

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

  std::string delta_imm = std::to_string(delta);
  for (int i = 0, size = delta_imm.size(); i < size; ++i) {
    buffer_[Xy(i, Bot() - 2)] = delta_imm[i];
  }
  std::string sin_imm = std::to_string(sin(time_lived * delta * 10.0) / 2.0);
  for (int i = 0, size = sin_imm.size(); i < size; ++i) {
    buffer_[Xy(i, Bot() - 1)] = sin_imm[i];
  }
  std::string dfs = std::to_string(time_lived);
  for (int i = 0, size = dfs.size(); i < size; ++i) {
    buffer_[Xy(i, Bot())] = dfs[i];
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
