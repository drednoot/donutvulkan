#include "core/result.h"

namespace core {

const char* CoreErrorToString(CoreError e) {
  switch (e) {
    case kCouldNotInitializeGlfwWindow:
      return "Could not initialize GLFW Window";
  }

  return "Unknown error code";
}

}  // namespace core
