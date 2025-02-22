#include "abstract_renderer.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "config.h"

AbstractRenderer::AbstractRenderer()
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
  z_buffer_.resize(width_ * height_);

  screen_ratio_ = width_ / (double)height_;
}

void AbstractRenderer::Start() {
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

void AbstractRenderer::Clear() {
  for (int i = 0; i < width_ * height_; ++i) {
    buffer_[i] = ' ';
    z_buffer_[i] = 0.0;
  }
}

void AbstractRenderer::Put(quat::Vec3 point, char sym) {
  int x_denorm = (int)(point.x * width_);
  if (x_denorm < 0 || x_denorm > Right()) {
    return;
  }
  int y_denorm = (int)(point.y * height_);
  if (y_denorm < 0 || y_denorm > Bot()) {
    return;
  }

  if (z_buffer_[Xy(x_denorm, y_denorm)] < point.z) {
    z_buffer_[Xy(x_denorm, y_denorm)] = point.z;
    buffer_[Xy(x_denorm, y_denorm)] = sym;
  }
}

void AbstractRenderer::DrawBuffer() const {
  MoveCursorTo(0, 0);
  std::cout.write(buffer_.data(), buffer_.size());
  std::cout.flush();
}

void AbstractRenderer::MoveCursorTo(int x, int y) const {
  printf("\033[%d;%dH", y, x);
}
