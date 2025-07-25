#include <cstdio>
#include <iostream>
#include <string>

#include <git2.h>

#include "context.hpp"
#include "modules/install.hpp"
#include "modules/validator.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/format_check.hpp"

int install() {
    if (!is_valid_url()) {
        std::cout << "The provided value is not a URL!" << std::endl;
        return 1;
    }

    size_t last_slash = ctx->name.find_last_of("/");
    if (last_slash == std::string::npos)
        return 1;

    std::string name = ctx->name.substr(last_slash + 1);

    if (name.size() >= 4 && name.compare(name.size() - 4, 4, ".git") == 0)
        name.erase(name.size() - 4);

    git_libgit2_init();

    const git_error *err = git_error_last();
    if (err && std::string(err->message) != "no error") {
        std::cerr << "Something failed during the git initialization.\n" << err->message << std::endl;
        return 1;
    }

    std::string clone_path = dotfiles_path + name;
    git_repository *repo_ptr = nullptr;
    const int clone_return = git_clone(&repo_ptr, ctx->name.c_str(), clone_path.c_str(), NULL);

    if (clone_return != 0) {
        std::cerr << "Something wen't wrong during the cloning process.\n" << git_error_last()->message << std::endl;
        return 1;
    }

    git_repository_free(repo_ptr);
    git_libgit2_shutdown();

    std::cout << "Downloaded the config! Wanna validate it? (Y/n) ";

    std::string validate_choice;
    std::getline(std::cin, validate_choice);
    if (validate_choice.empty() || validate_choice == "n" || validate_choice == "N")
        return 0;

    const Config config = Config(name);
    const auto validation_result = config.validate();

    if (validation_result.has_errors())
        validation_result.print();

    return 0;
}
