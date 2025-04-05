#include "renderer.h"

#include <iostream>
#include <memory>

#include "core/result.h"

int main() {
  auto rend_exp = Renderer::New();
  if (!rend_exp) {
    core::Result result = rend_exp.error();
    switch (result.kind) {
      case core::kNoError:
        std::cout << "wait what the fuck" << std::endl;
        break;
      case core::kCoreError:
        std::cout << core::CoreErrorToString(result.error.core);
        break;
      case core::kVkError:
        std::cout << "Vulkan error " << result.error.vk;
        break;
    }

    return 1;
  }
  std::unique_ptr<Renderer> rend(*rend_exp);

  rend->Start();
}
