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

/*
 * checks std::expected for unexpected
 * returns std::unextpected
 * evaluates to expected value
 */
#define UNWRAP(expr)                       \
  ({                                       \
    const auto& exp = expr;                \
    if (!exp) {                            \
      return std::unexpected(exp.error()); \
    }                                      \
    *exp;                                  \
  })

/*
 * checks std::expected for unexpected
 * returns error of std::expected
 * evaluates to expected value
 */
#define UNWRAP_ERR(expr)    \
  ({                        \
    const auto& exp = expr; \
    if (!exp) {             \
      return exp.error();   \
    }                       \
    *exp;                   \
  })

/*
 * checks function for nonzero return
 * pushes the return value up on the stack as std::expected
 * does nothing on nonzero
 */
#define TRY(expr)                  \
  {                                \
    const auto exp = expr;         \
    if (exp) {                     \
      return std::unexpected(exp); \
    }                              \
  }

/*
 * checks function for nonzero return
 * pushes the return value up on the stack
 * does nothing on nonzero
 */
#define TRY_ERR(expr)      \
  {                        \
    const auto exp = expr; \
    if (exp) {             \
      return exp;          \
    }                      \
  }

}  // namespace core

#endif  // DONUTCPP_CORE_RESULT_H_
