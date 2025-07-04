#include "list.hpp"
#include "toml.hpp"
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <string>
#include <format>

void list() {
  const char* home = std::getenv("HOME");
  if (!home) {
    std::cerr << "HOME env variable not set.\n";
    return;
  }

  std::filesystem::path dir = std::string(home) + "/Dokumente/Projekte/dotplug/test_config/";

  size_t i = 1;
  for (const auto & entry : std::filesystem::directory_iterator(dir)) {
    auto config_path = entry.path() / "config.toml";
    toml::table config;

    try {
      config = toml::parse_file(config_path.string());
    } catch (const toml::parse_error& err) {
      std::cerr << "Failed to parse " << config_path << ": " << err.description() << "\n";
      continue;
    }

    std::string_view name = config["dotplug"]["name"].value_or(std::string_view("Unknown Name"));
    std::string_view description = config["dotplug"]["description"].value_or(std::string_view(""));
    std::string_view author = config["dotplug"]["author"].value_or(std::string_view(""));
    auto dependencies = config["dotplug"]["dependencies"];

    std::cout << format("[{}] {}:", i, name) << std::endl;
    if (!description.empty()) std::cout << format("        Description: {}", description) << std::endl;
    if (!author.empty()) std::cout << format("        Author: {}", author) << std::endl;
    
    if (toml::array* arr = dependencies.as_array()) {
      if (!arr->empty()) {
        std::cout << "        Dependencies: ";
        for (size_t y = 0; y < arr->size(); ++y) {
          const auto& el = (*arr)[y];
          if (el.is_string()) {
            std::cout << *el.value<std::string>();
            if (y != arr->size() - 1) std::cout << ", ";
          }
        }
        std::cout << std::endl;
      }
    }

    ++i;
  }
}
