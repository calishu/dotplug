#include "list.hpp"
#include "toml.hpp"
#include "config.hpp"
#include "settings.hpp"
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <string>
#include <format>

void list() {
  size_t i = 1;
  for (const auto & entry : std::filesystem::directory_iterator(std::filesystem::path(dotfiles_path))) {
    toml::table config;
    size_t error_code;

    std::pair<toml::table&, size_t&>(config, error_code) = parse_config(entry.path().filename().string());
    if (error_code == 1) return;

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
