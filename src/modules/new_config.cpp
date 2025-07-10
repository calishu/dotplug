#include "toml.hpp"
#include "globals.hpp"
#include "settings.hpp"
#include <filesystem>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>

int new_config(const std::vector<std::string> &deps) {
  std::string path = dotfiles_path + *p_value;

  if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
    std::cout << "The config folder already exists. Please try another one.\n";
    return 1;
  }

  std::error_code ec;
  std::filesystem::create_directory(path, ec);
  if (ec) {
    std::cerr << "Unexpected Error: " << ec.message() << "\n";
    return 1;
  }

  toml::table root;


  toml::table dotplug;
  const char* user = getenv("USER");
  
  dotplug.insert("name", *p_value);
  dotplug.insert("author", user ? user : "unknown");

  toml::array deps_array;
  for (const auto& dep : deps) {
    deps_array.push_back(dep);
  }
  dotplug.insert("dependencies", deps_array);

  root.insert("dotplug", std::move(dotplug));

  std::ofstream ofs(path + "/config.toml");
  ofs << root;
  ofs.close();

  return 0;
}
