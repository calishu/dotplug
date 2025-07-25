#include <cstdlib>
#include <filesystem>
#include <string>

#include "context.hpp"
#include "modules/list.hpp"
#include "settings.hpp"
#include "utils/config.hpp"

void list() {
  toml::table config;

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
