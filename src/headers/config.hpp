#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "toml++/toml.hpp"

class Config {
private: 
  void parse_config();
public:
  std::string name_;
  toml::table config_;

  Config(const std::string& name);

  std::vector<std::string> get_dependencies() const; // get all dependencies
  std::unordered_map<std::string, std::string> get_dependency(const std::string& dep_name) const; // get data from a specific dependency
};

void print_config(Config& config, const std::string prefix = "");
