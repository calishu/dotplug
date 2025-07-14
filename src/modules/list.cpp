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
    std::pair<toml::table&, int&>(config, error_code) = parse_config(ctx->name);
    if (error_code == 1) return;

    print_config(config);
    
    return;
  }

  size_t i = 1;
  for (const auto & entry : std::filesystem::directory_iterator(std::filesystem::path(dotfiles_path))) {
    std::pair<toml::table&, int&>(config, error_code) = parse_config(entry.path().filename().string());
    if (error_code == 1) return;

    print_config(config, std::to_string(i));
    
    ++i;
  }
}
