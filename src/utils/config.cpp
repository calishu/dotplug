#include "toml++/toml.hpp"
#include "settings.hpp"
#include "context.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

class Config {
private:
  std::string name_;
  toml::table config_;

  void parse_config(std::string name) {
    std::string path = dotfiles_path + name + "/config.toml";

    if(!std::filesystem::exists(path)) {
      std::cerr << "The config doesn't have a config.toml" << std::endl;
      return;
    }

    try {
      config_ = toml::parse_file(path);
    } catch (const toml::parse_error& err) {
      std::cerr << "Parsing failed: \n" << err << std::endl;
    }
    return;
  }

public:
  Config(const std::string& name) : name_(name) {
    parse_config(name);
  }

  const std::vector<std::string>& get_dependencies() {
    static std::vector<std::string> dependencies;
    dependencies.clear();

    const auto& config_deps = config["dotplug"]["dependencies"];
    const auto* deps_array  = config_deps.as_array();

    if (!deps_array || deps_array->empty())
      return dependencies;

    for (size_t i = 0; i < deps_array->size(); ++i) {
      const auto& dep = (*arr)[i];

      if (!dep.is_string()) continue;
      dependencies.push_back(dep);
    }

    return dependencies;
  }
}
