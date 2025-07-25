#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <toml++/toml.hpp>

class Config {
    std::string name_;
    toml::table config_;

public:
    Config(const std::string &name);

    // get all dependencies
    auto get_dependencies() const -> std::vector<std::string>;

    // get data from a specific dependency
    auto get_dependency(const std::string &dep_name) const -> std::unordered_map<std::string, std::string>;

    auto name() const -> const std::string & { return name_; }
    auto config() const -> const toml::table & { return config_; }

    // formerly a free function called print_config(const Config &, ...)
    auto print(const std::string prefix = "") const -> void;

private:
    auto parse_config() -> void;
};
