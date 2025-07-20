#include "list.hpp"
#include "config.hpp"
#include "context.hpp"
#include "settings.hpp"
#include "toml++/toml.hpp"
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <string>
#include <format>

void list() {
  toml::table config;
  int error_code;

  if (!ctx->name.empty()) {
    Config config_ = Config(ctx->name);

    print_config(config_);
    
    return;
  }

  size_t i = 1;
  for (const auto & entry : std::filesystem::directory_iterator(std::filesystem::path(dotfiles_path))) {
    Config config_ = Config(entry.path().filename().string());

    print_config(config_, std::to_string(i));
    
    ++i;
  }
}
