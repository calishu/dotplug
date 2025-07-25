#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include <toml++/toml.hpp>

#include "context.hpp"
#include "settings.hpp"

namespace fs = std::filesystem;

int new_config(const std::vector<std::string> &deps) {
    const auto path = dotfiles_path + ctx->name;

    if (fs::is_directory(path)) {
        std::cout << "The config folder already exists. Please try another one.\n";
        return 1;
    }

    fs::create_directory(path);

    toml::table root, dotplug;
    const auto user = getenv("USER");

    dotplug.insert("name", ctx->name);
    dotplug.insert("author", user ? user : "unknown");

    toml::array deps_array;
    for (const auto &dep : deps)
        deps_array.emplace_back(dep);
    dotplug.insert("dependencies", deps_array);

    root.insert("dotplug", std::move(dotplug));

    std::ofstream{path + "/config.toml"} << root;
    return 0;
}
