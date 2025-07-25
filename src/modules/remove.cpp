#include <filesystem>
#include <iostream>
#include <string>

#include "context.hpp"
#include "settings.hpp"

namespace fs = std::filesystem;

int remove_config() {
    const auto path = dotfiles_path + ctx->name;

    if (!fs::exists(path)) {
        std::cout << "The config doesn't exist.\n";
        return 1;
    }

    if (!ctx->forced) {
        std::string choice;
        std::cout << "Do you really want to delete the config? (y/N) ";
        std::getline(std::cin, choice);
        if (choice != "y" && choice != "Y")
            return 0;
    }

	fs::remove_all(path);
    return 0;
}
