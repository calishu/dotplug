#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "toml++/toml.hpp"

std::pair<toml::table, int> parse_config(const std::string& name);
std::pair<std::string, int> dep_source(const toml::table& config, const std::string& dep_name);
void print_config(toml::table& config, const std::string prefix = "");

class Config {
private:
  std::string name_;
  toml::table config_;
  
  void parse_config(const std::string& name);
public:
  Config(const std::string& name);

  std::vector<std::string> get_dependencies() const; // get all dependencies
  std::unordered_map<std::string, std::string> get_dependency(const std::string& dep_name) const; // get data from a specific dependency
};
