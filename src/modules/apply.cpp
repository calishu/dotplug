#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

#include "context.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/formatting.hpp"
#include "utils/logging.hpp"
#include "utils/symlinks.hpp"

namespace fs = std::filesystem;

auto apply() -> int {
    if (fs::exists(state_path + "current_configuration"))
        remove_all_symlinks();

    const Config config{ctx->name};

    for (const auto &dep_name : config.get_dependencies()) {
        const auto dep = config.get_dependency(dep_name);
        const auto src = dep.at("source"), dst = dep.at("destination");

        if (!fs::exists(src)) {
            ctx->logging->log(
                LoggingLevel::ERROR, replace_format(ctx->locale.json["apply"]["errors"]["source_not_found"], dep_name));
            return 1;
        }

        if (dst.empty()) {
            ctx->logging->log(
                LoggingLevel::ERROR,
                replace_format(ctx->locale.json["apply"]["errors"]["destination_not_found"], dep_name));
            return 1;
        }

        fs::create_directory_symlink(src, dst);
    }

    std::ofstream{state_path + "current_configuration"} << (dotfiles_path + config.name() + "/config.toml");
    ctx->logging->log(LoggingLevel::INFO, ctx->locale.json["apply"]["statements"]["success"]);
    return 0;
}
