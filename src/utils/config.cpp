#include "toml.hpp"
#include "settings.hpp"
#include <filesystem>
#include <iostream>
#include <string>

std::pair<toml::table, size_t> parse_config(const std::string& name) { 
  toml::table config;

  try {
    config = toml::parse_file(dotfiles_path + name + "/config.toml");
  } catch (const toml::parse_error& err) {
    std::cerr << "Parsing failed: \n" << err << std::endl;
    return {toml::table{}, 1};
  }

  return {config, 0};
}
