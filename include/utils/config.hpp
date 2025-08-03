#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <toml++/toml.hpp>

#include "modules/validator.hpp"
#include "nlohmann/json.hpp"

class Config {
    std::string name_;
    toml::table config_;
    nlohmann::json lang;
    std::locale locale;

public:
    Config(const std::string &name);

    // get all dependencies
    auto get_dependencies() const -> std::vector<std::string>;

    // get data from a specific dependency
    auto get_dependency(const std::string &dep_name) const -> std::unordered_map<std::string, std::string>;

    auto print(const std::string &prefix = "") const -> void;
    auto validate() const -> ValidationResult;

    // getter methods
    auto name() const { return name_; }
    auto config() const { return config_; }

private:
    auto parse_config() -> void;
};
