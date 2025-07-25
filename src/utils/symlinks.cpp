#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "context.hpp"
#include "settings.hpp"
#include "utils/config.hpp"

namespace fs = std::filesystem;

/* @brief Removes every active symlink from the config!
 *
 * The `current` file in the base config path, contains the name of the active
 * config.
 *
 * @return the return code 0 for success, 1 for fail/error.
 * */
int remove_all_symlinks() {
    std::string active_config_name;

    std::ifstream current{config_path + "current"};
    if (current.good()) {
        std::getline(current, active_config_name);
    } else {
        std::cerr << "There is no active config!\n";
        return 1;
    }

    const Config active_config{active_config_name};
    const auto dependencies = active_config.get_dependencies();

    for (const auto &dep_name : dependencies) {
        const auto dep = active_config.get_dependency(dep_name);
		const auto dst = dep.at("destination");

        if (!fs::exists(dst)) {
            std::cout << "[ERROR] Couldn't find the destination.\n";
            return 1;
        }

        if (!fs::is_symlink(dst) && ctx->forced == false) {
            std::cout << "[WARNING] " << dst << "isn't a symlink! Want to delete it? (y/N)";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "n" || choice == "N" || choice == "")
                continue;
        }

		fs::remove(dst);
    }

    fs::remove(config_path + "current");
    return 0;
}
