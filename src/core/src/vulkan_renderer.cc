#include "core/vulkan_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>
#include <expected>
#include <memory>

#include "core/result.h"

#include "vulkan_renderer_impl.h"

namespace core {

std::expected<VulkanRenderer*, Result> VulkanRenderer::New(
    const VulkanRendererConfig& config) {
  std::unique_ptr<VulkanRenderer> rend(new VulkanRenderer);

  TRY_RESULT_NO_ERROR(rend->d->New(config));

  return rend.release();
}

VulkanRenderer::VulkanRenderer() : d(std::make_unique<Impl>()) {}

VulkanRenderer::~VulkanRenderer() = default;

void VulkanRenderer::Start() {
  d->Start();
}

void VulkanRenderer::Clear() {
  for (int i = 0; i < d->cfg_.width * d->cfg_.height; ++i) {
    d->buffer_[i] = ' ';
    d->z_buffer_[i] = 0.0;
  }
}

void VulkanRenderer::Put(int x, int y, char sym) {
  d->buffer_[Xy(x, y)] = sym;
}

void VulkanRenderer::Put(const core::Vec3& point, char sym) {
  int x_denorm = (int)(point.x * d->cfg_.width);
  if (x_denorm < 0 || x_denorm > Right()) {
    return;
  }
  int y_denorm = (int)(point.y * d->cfg_.height);
  if (y_denorm < 0 || y_denorm > Bot()) {
    return;
  }

  if (d->z_buffer_[Xy(x_denorm, y_denorm)] < point.z) {
    d->z_buffer_[Xy(x_denorm, y_denorm)] = point.z;
    d->buffer_[Xy(x_denorm, y_denorm)] = sym;
  }
}

int VulkanRenderer::GetWidth() const {
  return d->cfg_.width;
}

int VulkanRenderer::GetHeight() const {
  return d->cfg_.height;
}

double VulkanRenderer::GetRatio() const {
  return d->screen_ratio_;
}

int VulkanRenderer::Xy(int x, int y) const {
  return y * d->cfg_.width + x;
}

int VulkanRenderer::Right() const {
  return d->cfg_.width - 1;
}

int VulkanRenderer::Bot() const {
  return d->cfg_.height - 1;
}

double VulkanRenderer::ToAspect(double val) const {
  return val * GetRatio();
}

double VulkanRenderer::NsToSeconds(std::chrono::nanoseconds ns) const {
  const long count =
      std::chrono::duration_cast<std::chrono::milliseconds, long>(ns).count();
  return count / 1000.0;
}

double VulkanRenderer::TimeLived() const {
  std::chrono::nanoseconds now =
      std::chrono::system_clock::now().time_since_epoch();
  std::chrono::nanoseconds from_start = now - d->start_time_;
  return NsToSeconds(from_start);
}

}  // namespace core
