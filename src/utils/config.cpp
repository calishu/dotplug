#include "toml++/toml.hpp"
#include "context.hpp"
#include "settings.hpp"
#include <filesystem>
#include <iostream>
#include <string>

std::pair<toml::table, int> parse_config(const std::string& name) { 
  toml::table config;

  try {
    config = toml::parse_file(dotfiles_path + name + "/config.toml");
  } catch (const toml::parse_error& err) {
    std::cerr << "Parsing failed: \n" << err << std::endl;
    return {toml::table{}, 1};
  }

  return {config, 0};
}

// TODO: Make it better.
// Somehow I feel ashamed of writing this piece of code.
// I did a lot of bad code, but idk why I use global pointers for that.
std::pair<std::string, int> dep_source(const toml::table& config, const std::string& dep_name) {
  auto source_node = config["dotplug"][dep_name]["source"];
  std::string source_path = source_node.is_string() ? source_node.value_or("") : "";

  std::filesystem::path source = source_path.empty()
    ? dotfiles_path + ctx->name + "/" + dep_name
    : dotfiles_path + ctx->name + "/" + source_path;

  if (std::filesystem::exists(source) && std::filesystem::is_directory(source))
    return { std::filesystem::absolute(source), 0 };
  return { "", 1 };
}
