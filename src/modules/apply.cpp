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
    if (fs::exists(config_path + "current"))
        remove_all_symlinks();

    const Config config{ctx->name};

    for (std::string &dep_name : config.get_dependencies()) {
        const auto dep = config.get_dependency(dep_name);
        const auto src = dep.at("source"), dst = dep.at("destination");

        if (!fs::exists(src)) {
            std::cerr << "[ERROR] The source of " << dep_name << " couldn't be found!" << std::endl;
            return 1;
        }

        if (dst.empty()) {
            std::cerr << "[ERROR] Couldn't find the destination of " << dep_name << std::endl;
            return 1;
        }

        fs::create_directory_symlink(src, dst);
    }

    std::ofstream current{config_path + "current"};
    current << (dotfiles_path + config.name() + "/config.toml");

    return 0;
}
