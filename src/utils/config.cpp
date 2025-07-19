#include "toml++/toml.hpp"
#include "settings.hpp"
#include "context.hpp"
#include <unordered_map>
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
    parse_config(name_);
  }

  // Get the list of dependencies
  const std::vector<std::string> get_dependencies() {
    std::vector<std::string> dependencies;

    const auto& config_deps = config_["dotplug"]["dependencies"];
    const auto* deps_array  = config_deps.as_array();

    if (!deps_array || deps_array->empty())
      return dependencies;

    for (size_t i = 0; i < deps_array->size(); ++i) {
      const auto& dep_node = (*deps_array)[i];

      if (!dep_node.is_string()) continue;
      dependencies.push_back(dep_node.value<std::string>().value_or(""));
    }

    return dependencies;
  }

  // Get the data of a specific dependency
  const std::unordered_map<std::string, std::string> get_dependency(const std::string& dep_name) {
    std::unordered_map<std::string, std::string> output;

    if (!config_.contains(dep_name) || !config_[dep_name].is_table()) {
      output["error"] = (dep_name + " doesn't exist or isn't a table!");
      return output;
    }

    auto source_node = config_["dotplug"][dep_name]["source"];
    auto destination_node = config_["dotplug"][dep_name]["destination"];

    std::string source_path = source_node.is_string() ? source_node.value_or("") : "";
    std::string dest_path = destination_node.is_string() ? destination_node.value_or("") : "";

    std::string source = source_path.empty()
      ? dotfiles_path + name_ + "/" + dep_name
      : dotfiles_path + name_ + "/" + source_path;

    std::string destination = dest_path.empty()
      ? destination_path + name_ + "/" + dep_name
      : destination_path + name_ + "/" + dest_path;

    output["source"] = source;
    output["destination"] = destination;

    return output;
  }
};
