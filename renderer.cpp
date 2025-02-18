#include "renderer.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

Renderer::Renderer() : width_(0), height_(0) {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  width_ = w.ws_col;
  height_ = w.ws_row;
  buffer_.resize(width_ * height_);
}

void Renderer::Start() {
  Clear();

  DrawBuffer();
}

void Renderer::Clear() {
  for (int i = 0; i < width_ * height_; ++i) {
    buffer_[i] = ' ';
  }
}

void Renderer::DrawBuffer() {
  MoveCursorTo(0, 0);
  std::cout.write(buffer_.data(), buffer_.size());
  std::cout.flush();
}

void Renderer::MoveCursorTo(int x, int y) {
  printf("\033[%d;%dH", y, x);
}
