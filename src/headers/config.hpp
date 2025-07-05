#pragma once

#include <string>
#include "toml.hpp"

std::pair<toml::table, int> parse_config(const std::string& name);
