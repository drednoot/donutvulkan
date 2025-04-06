#include "renderer.h"

#include <iostream>
#include <memory>

#include "core/result.h"

int main() {
  auto rend_exp = Renderer::New();
  if (!rend_exp) {
    std::cout << core::ResultToString(rend_exp.error()) << std::endl;

    return 1;
  }
  std::unique_ptr<Renderer> rend(*rend_exp);

  rend->Start();
}
