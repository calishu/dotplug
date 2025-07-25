#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/config.hpp"

struct ValidationResult {
    enum class Type { Warning, Error };

    struct Entries {
        std::string message;
        Type type;
    };

    std::vector<Entries> log;

    void add_error(std::string msg) { log.push_back({msg, Type::Error}); }
    void add_warning(std::string msg) { log.push_back({msg, Type::Warning}); }
};

void print_validation(ValidationResult &result) {
    if (result.log.empty())
        std::cout << "Everything is fine with the configuration." << std::endl;
    for (const auto &e : result.log)
        std::cout << (e.type == ValidationResult::Type::Error ? "[ERROR] " : "[WARNING] ") << e.message << "\n";
    return;
}

ValidationResult validator(const Config &config) {
    ValidationResult output;

    const std::vector<std::string> dependencies = config.get_dependencies();

    if (dependencies.empty()) {
        std::cout << "You need at least a single dependency" << std::endl;
        return output;
    }

    for (const std::string &dep : dependencies) {
        if (!config.config_["dotplug"][dep].is_table()) {
            output.add_error(dep + " doesn't exist or it isn't a table");
            continue;
        }

        const std::unordered_map<std::string, std::string> dep_infos = config.get_dependency(dep);

        if (dep_infos.count("destination") && (dep_infos.at("destination").empty()))
            output.add_error("You need to add destination for " + dep);

        if (dep_infos.count("source") &&
            (dep_infos.at("source").empty() || !std::filesystem::exists(dep_infos.at("source")))) {
            output.add_error("Couldn't find source of " + dep);
        }
    }

    return output;
}
