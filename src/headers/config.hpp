#pragma once

#include <string>
#include "toml.hpp"

std::pair<toml::table, int> parse_config(const std::string& name);
std::pair<std::string, int> dep_source(const toml::table& config, const std::string& dep_name);
