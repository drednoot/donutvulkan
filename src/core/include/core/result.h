#ifndef DONUTCPP_CORE_RESULT_H_
#define DONUTCPP_CORE_RESULT_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <expected>
#include <string>

namespace core {

enum ResultKind {
  kNoError = 0,
  kCoreError,
  kVkError,
  kFileError,
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

struct FileError {
  const char* filename;
};

const std::string CoreErrorToString(CoreError e);

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
  Result(FileError file_error)
      : kind(kFileError), error({.file = file_error}) {}

  operator bool() const { return kind == kNoError; };

  ResultKind kind;
  union {
    NoError no_error;
    CoreError core;
    VkResult vk;
    FileError file;
  } error;
};

const std::string ResultToString(Result r);

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

/*
 * checks function for good Result return
 * pushes the return value up on the stack as std::expected
 * does nothing on good value
 */
#define TRY_RS(expr)               \
  {                                \
    core::Result res = expr;       \
    if (!res) {                    \
      return std::unexpected(res); \
    }                              \
  }

/*
 * checks function for good Result return
 * pushes the return value up on the stack
 * does nothing on good value
 */
#define TRY_RS_ERR(expr)     \
  {                          \
    core::Result res = expr; \
    if (!res) {              \
      return res;            \
    }                        \
  }

}  // namespace core

#endif  // DONUTCPP_CORE_RESULT_H_
