#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "settings.hpp"
#include "utils/config.hpp"

namespace fs = std::filesystem;

Config::Config(const std::string &name)
    : name_(name) {
    parse_config();
}

auto Config::parse_config() -> void {
    const auto path = dotfiles_path + name_ + "/config.toml";

    if (!fs::exists(path))
        throw std::runtime_error{"The config doesn't have a config.toml"};

    try {
        config_ = toml::parse_file(path);
    } catch (const toml::parse_error &err) {
        throw std::runtime_error{"Parsing failed: \n" + std::string{err.description()}};
    }

    return;
}

// Get the list of dependencies
auto Config::get_dependencies() const -> std::vector<std::string> {
    auto dependencies     = std::vector<std::string>{};
    const auto deps_array = config_["dotplug"]["dependencies"].as_array();

    if (!deps_array || deps_array->empty())
        return dependencies;

    for (const auto &dep_node : *deps_array) {
        if (!dep_node.is_string())
            continue;
        dependencies.emplace_back(dep_node.value_or<std::string>(""));
    }

    return dependencies;
}

// Get the data of a specific dependency
auto Config::get_dependency(const std::string &dep_name) const -> std::unordered_map<std::string, std::string> {
    auto output = std::unordered_map<std::string, std::string>{};

    if (!config_["dotplug"][dep_name].is_table())
        throw std::runtime_error{dep_name + " doesn't exist or isn't a table"};

    const auto source_node      = config_["dotplug"][dep_name]["source"];
    const auto destination_node = config_["dotplug"][dep_name]["destination"];

    // formatter doesn't respect existing ternary breaking, so just turn it off
    // clang-format off
    const auto source_path = (source_node || source_node.is_string())
        ? source_node.value_or("")
        : dep_name;

    const auto dest_path = (destination_node || destination_node.is_string())
        ? destination_node.value_or("")
        : dep_name;

    const auto source = source_path.empty()
        ? (dotfiles_path + name_ + "/" + dep_name)
        : (dotfiles_path + name_ + "/" + source_path);

    const auto destination = dest_path.empty() 
        ? (destination_path + dep_name)
        : (destination_path + dest_path);
    // clang-format on

    output["source"]      = source;
    output["destination"] = destination;

    return output;
}

auto Config::print(const std::string prefix) const -> void {
    const auto name         = config_["dotplug"]["name"].value_or<std::string>("Unknown Name"),
               description  = config_["dotplug"]["description"].value_or<std::string>(""),
               author       = config_["dotplug"]["author"].value_or<std::string>("");
    const auto dependencies = get_dependencies();

    std::cout << (!prefix.empty() ? "[" + prefix + "] " : "") << name << ":\n";
    if (!description.empty())
        std::cout << "        Description: " << description << "\n";
    if (!author.empty())
        std::cout << "        Author: " << author << "\n";

    std::cout << "        Dependencies: ";
    for (const auto &i : dependencies)
        std::cout << i << ", ";
    std::cout << "\n";
}

auto Config::validate() const -> ValidationResult {
    auto output             = ValidationResult{};
    const auto dependencies = get_dependencies();

    if (dependencies.empty()) {
        std::cout << "You need at least a single dependency\n";
        return output;
    }

    for (const auto &dep : dependencies) {
        if (!config_["dotplug"][dep].is_table()) {
            output.add_error(dep + " doesn't exist or it isn't a table");
            continue;
        }

        const auto dep_infos = get_dependency(dep);

        if (dep_infos.count("destination") && (dep_infos.at("destination").empty()))
            output.add_error("You need to add destination for " + dep);

        if (dep_infos.count("source") && (dep_infos.at("source").empty() || !fs::exists(dep_infos.at("source"))))
            output.add_error("Couldn't find source of " + dep);
    }

    return output;
}
