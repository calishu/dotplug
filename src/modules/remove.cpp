#include <filesystem>
#include <iostream>
#include <string>

#include "context.hpp"
#include "settings.hpp"

int remove_config() {
    std::string path = dotfiles_path + ctx->name;

    if (!std::filesystem::exists(path)) {
        std::cout << "The config doesn't exist." << std::endl;
        return 1;
    }

    if (!ctx->forced) {
        std::string choice;
        std::cout << "Do you really want to delete the config? (y/N) ";
        std::getline(std::cin, choice);
        if (choice != "y" && choice != "Y")
            return 0;
    }

    std::error_code err;
    if (!std::filesystem::remove_all(path, err)) {
        std::cout << "Oopsies, a error happened :( \n" << err.message() << std::endl;
        return 1;
    }

    return 0;
}
