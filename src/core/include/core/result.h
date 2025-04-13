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
};

const char* CoreErrorToString(CoreError e);

/**
 * Result type is used to have the result of an operation.
 * Default constructor means no error occurred
 * (Result().kind == ResultKind::kNoError).
 * other constructors are explicit
 */
struct Result {
  Result() : kind(kNoError), error({.no_error = NoError{}}) {}
  explicit Result(CoreError core_error)
      : kind(kCoreError), error({.core = core_error}) {}
  explicit Result(VkResult vk_error)
      : kind(kVkError), error({.vk = vk_error}) {}

  ResultKind kind;
  union {
    NoError no_error;
    CoreError core;
    VkResult vk;
  } error;
};

const char* ResultToString(Result r);

#define TRY(expr)                                        \
  ({                                                     \
    const auto& exp = expr;                              \
    if (!exp) {                                          \
      return std::unexpected(core::Result(exp.error())); \
    }                                                    \
    *exp;                                                \
  })

}  // namespace core

#endif  // DONUTCPP_CORE_RESULT_H_
