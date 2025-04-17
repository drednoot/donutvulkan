#ifndef DONUTCPP_CORE_RESULT_H_
#define DONUTCPP_CORE_RESULT_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>

namespace core {

enum ResultKind {
  kNoError = 0,
  kCoreError,
  kVkError,
};

struct NoError {};

enum CoreError {
  kCouldNotInitializeGlfwWindow = 0,
  kNoGpuWithVulkanSupport,
  kNoSuitableGpu,
  kNotAllRequiredQueueFamiliesArePresent,
  kNoAvailableSurfaceFormats,
  kNoAvailableSurfacePresentModes,
};

const char* CoreErrorToString(CoreError e);

/**
 * Result type is used to have the result of an operation.
 * Default constructor means no error occurred
 * (Result().kind == ResultKind::kNoError).
 */
struct Result {
  Result() : kind(kNoError), error({.no_error = NoError{}}) {}
  Result(CoreError core_error)
      : kind(kCoreError), error({.core = core_error}) {}
  Result(VkResult vk_error) : kind(kVkError), error({.vk = vk_error}) {}

  operator bool() const { return kind == kNoError; };

  ResultKind kind;
  union {
    NoError no_error;
    CoreError core;
    VkResult vk;
  } error;
};

const char* ResultToString(Result r);

#define TRY(expr)                          \
  ({                                       \
    const auto& exp = expr;                \
    if (!exp) {                            \
      return std::unexpected(exp.error()); \
    }                                      \
    *exp;                                  \
  })

#define TRY_VK_SUCCESS(expr)       \
  {                                \
    VkResult res = expr;           \
    if (res != VK_SUCCESS) {       \
      return std::unexpected(res); \
    }                              \
  }

#define TRY_RESULT_NO_ERROR(expr)                 \
  {                                               \
    Result res = expr;                            \
    if (res.kind != core::ResultKind::kNoError) { \
      return std::unexpected(res);                \
    }                                             \
  }

#define TRY_RESULT(expr)    \
  ({                        \
    const auto& exp = expr; \
    if (!exp) {             \
      return exp.error();   \
    }                       \
    *exp;                   \
  })

}  // namespace core

#endif  // DONUTCPP_CORE_RESULT_H_
