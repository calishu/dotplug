#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

#include <toml++/toml.hpp>

#include "context.hpp"
#include "settings.hpp"
#include "utils/formatting.hpp"

namespace fs = std::filesystem;

int new_config(const std::string &name, const std::vector<std::string> &deps) {
    auto *logging   = ctx->logging;
    auto locale     = &ctx->locale;
    const auto path = dotfiles_path + name;

    if (fs::is_directory(path)) {
        logging->log(LoggingLevel::ERROR, locale->json["new_cfg"]["errors"]["already_exists"]);
        return 1;
    }

    fs::create_directory(path);

    toml::table root, dotplug;
    const auto user = getlogin();

    dotplug.insert("name", name);
    dotplug.insert("author", user ? user : "unknown");

    if (deps.empty())
        auto deps = std::get<std::vector<std::string>>(
            logging->prompt(PromptMode::LIST, locale->json["new_cfg"]["prompts"]["dependencies"], "false"));

    if (!deps.empty()) {
        toml::array deps_array;

        for (const auto &dep : deps) {
            deps_array.emplace_back(dep);

            std::string location_prompt = locale->json["new_cfg"]["prompts"]["location"];
            location_prompt             = replace_format(location_prompt, dep);

            auto destination = std::get<std::string>(logging->prompt(PromptMode::STRING, location_prompt, ""));

            fs::path fs_destination(resolve_path(destination));
            fs::path fs_destination_parent = fs_destination.parent_path();

            if (!fs::exists(fs_destination_parent)) {
                logging->log(LoggingLevel::ERROR, locale->json["new_cfg"]["errors"]["not_valid_path"]);
                return 1;
            }

            fs::create_directory(path + dep);

            toml::table dep_table;
            dep_table.insert("source", (path + dep));
            dep_table.insert("destination", fs::absolute(fs_destination / dep).string());

            dotplug.insert(dep, std::move(dep_table));

            if (!fs::exists(fs_destination))
                continue;

            // TODO: check if destination exists and ask if copy
            std::string copy_prompt = locale->json["new_cfg"]["prompts"]["copy"];
            copy_prompt             = replace_format(copy_prompt, dep);

            auto ask_copy = std::get<bool>(logging->prompt(PromptMode::BOOL, copy_prompt, "0"));

            if (ask_copy) {
                fs::copy(fs_destination, (path + dep));
            } else {
                auto ask_backup = std::get<bool>(
                    logging->prompt(PromptMode::BOOL, locale->json["new_cfg"]["prompts"]["backup"], "0"));
                // TODO: integrate the (coming) backup system
            }
        }

        dotplug.insert("dependencies", deps_array);
    }

    root.insert("dotplug", std::move(dotplug));

    std::ofstream{path + "/config.toml"} << root;
    return 0;
}
