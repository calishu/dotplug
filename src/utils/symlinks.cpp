#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "context.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/formatting.hpp"
#include "utils/logging.hpp"

namespace fs = std::filesystem;

/* @brief Removes every active symlink from the config!
 *
 * The `current` file in the base config path, contains the name of the active
 * config.
 *
 * @return the return code 0 for success, 1 for fail/error.
 * */
int remove_all_symlinks() {
    auto &logging = ctx->logging;
    auto &lang    = ctx->locale.json;

    std::string active_config_name;

    std::ifstream current{state_path + "current_configuration"};
    if (current.good()) {
        std::getline(current, active_config_name);
    } else {
        logging->log(LoggingLevel::ERROR, lang["rm_symlinks"]["errors"]["no_active_config"]);
        return 1;
    }

    const Config active_config{active_config_name};
    const auto dependencies = active_config.get_dependencies();

    for (const auto &dep_name : dependencies) {
        const auto dep = active_config.get_dependency(dep_name);
        const auto dst = dep.at("destination");

        if (!fs::exists(dst)) {
            logging->log(
                LoggingLevel::ERROR, replace_format(lang["rm_symlinks"]["errors"]["destination_not_found"], dep_name));
            return 1;
        }

        if (!fs::is_symlink(dst) && !ctx->forced) {
            if (std::get<bool>(logging->prompt(
                    PromptMode::BOOL, replace_format(lang["rm_symlinks"]["errors"]["not_a_symlink"], dep_name), "0")))
                continue;
        }

        fs::remove(dst);
    }

    fs::remove(state_path + "current_configuration");
    return 0;
}
