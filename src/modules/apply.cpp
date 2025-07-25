#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "context.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/symlinks.hpp"

namespace fs = std::filesystem;

int apply() {
    if (fs::exists(state_path + "current_configuration"))
        remove_all_symlinks();

    const Config config{ctx->name};

    for (const auto &dep_name : config.get_dependencies()) {
        const auto dep = config.get_dependency(dep_name);
        const auto src = dep.at("source"), dst = dep.at("destination");

        if (!fs::exists(src)) {
            std::cerr << "[ERROR] The source of " << dep_name << " couldn't be found!\n";
            return 1;
        }

        if (dst.empty()) {
            std::cerr << "[ERROR] Couldn't find the destination of " << dep_name << '\n';
            return 1;
        }

        fs::create_directory_symlink(src, dst);
    }

    std::ofstream{state_path + "current_configuration"} << (dotfiles_path + config.name() + "/config.toml");
    return 0;
}
