#pragma once

#include <string>

std::pair<toml::table, int> parse_config(const std::string& name);
