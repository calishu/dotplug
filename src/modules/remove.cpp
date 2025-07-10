#include "settings.hpp"
#include "context.hpp"
#include <filesystem>
#include <iostream>
#include <string>

int remove_config() {
  std::string path = dotfiles_path + ctx->name;

  if (!std::filesystem::exists(path)) {
    std::cout << "The config doesn't exist." << std::endl;
    return 1;
  }

  std::error_code err;
  if (!std::filesystem::remove_all(path, err)) {
    std::cout << "Oopsies, a error happened :( \n" << err.message() << std::endl;
    return 1;
  }

  return 0;
}
