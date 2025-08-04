#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "context.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/formatting.hpp"

namespace fs = std::filesystem;

Config::Config(const std::string &name)
    : name_(name) {
    lang   = ctx->locale.json;
    locale = ctx->locale.locale;
    parse_config();
}

auto Config::parse_config() -> void {
    const auto path = dotfiles_path + name_ + "/config.toml";

    if (!fs::exists(path)) {
        std::cout << path << "\n";
        throw std::runtime_error{lang["config"]["errors"]["no_config_toml"]};
    }

    try {
        config_ = toml::parse_file(path);
    } catch (const toml::parse_error &err) {
        throw std::runtime_error{
            std::string{lang["config"]["errors"]["parsing_failed"]} + " \n" + std::string{err.description()}};
    }
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
        throw std::runtime_error{dep_name + " " + std::string(lang["config"]["errors"]["doesnt_exist_or_no_table"])};

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

auto Config::print(const std::string &prefix) const -> void {
    const auto name         = config_["dotplug"]["name"].value_or<std::string>("Unknown Name"),
               description  = config_["dotplug"]["description"].value_or<std::string>(""),
               author       = config_["dotplug"]["author"].value_or<std::string>("");
    const auto dependencies = get_dependencies();

    std::string indent = "        ";
    std::cout << (!prefix.empty() ? "[" + prefix + "] " : "") << name << ":\n";

    if (!description.empty())
        std::cout << indent << lang["config"]["words"]["description"] << ": " << description << "\n ";
    if (!author.empty())
        std::cout << indent << lang["config"]["words"]["author"] << ": " << author << "\n";

    std::cout << indent << lang["config"]["words"]["dependencies"] << ": ";

    size_t i = 0;
    for (const auto &dep : dependencies) {
        std::cout << dep;
        if (i < dependencies.size() - 1)
            std::cout << ", ";
        ++i;
    }

    std::cout << "\n";
}

auto Config::validate() const -> ValidationResult {
    auto output             = ValidationResult{};
    const auto dependencies = get_dependencies();

    if (dependencies.empty()) {
        output.add_error(lang["config"]["errors"]["no_deps"]);
        return output;
    }

    for (const auto &dep : dependencies) {
        if (!config_["dotplug"][dep].is_table()) {
            output.add_error(replace_format(lang["config"]["errors"]["doesnt_exist_or_no_table"], dep));
            continue;
        }

        const auto dep_infos = get_dependency(dep);

        if (dep_infos.contains("destination") && (dep_infos.at("destination").empty()))
            output.add_error(replace_format(lang["validation"]["errors"]["destination_required"], dep));

        if (dep_infos.contains("source") && (dep_infos.at("source").empty() || !fs::exists(dep_infos.at("source"))))
            output.add_error(replace_format(lang["validation"]["errors"]["source_not_found"], dep));
    }

    return output;
}
