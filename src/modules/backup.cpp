#include <filesystem>
#include <iostream>
#include <unistd.h>

#include <git2.h>
#include <git2/commit.h>
#include <git2/repository.h>
#include <git2/signature.h>

#include "modules/backup.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/git.hpp"

namespace fs = std::filesystem;

Backup::Backup(const Config &config)
    : config_(config) {
    git_libgit2_init();

    backup_path_ = backup_path + config_.name();

    if (!fs::exists(backup_path_) || !fs::exists(backup_path_ + "/.git")) {
        fs::create_directory(backup_path_);

        git_repository *repo_ = nullptr;
        if (git_repository_init(&repo_, backup_path_.c_str(), 0) != 0) {
            const git_error *err = git_error_last();
            std::cerr << "[ERROR] Failed to initialize repo: \n" << err->message << "\n";
            return;
        }

        std::error_code symlink_err;
        fs::create_directory_symlink((dotfiles_path + config_.name()), backup_path_, symlink_err);
        if (symlink_err) {
            std::cerr << "Failed to create a symlink pointing to the config. \n" << symlink_err.message() << "\n";
            return;
        }
    }
}

auto Backup::create(const std::string &description) const -> int {
    if (create_commit(backup_path_, (dotfiles_path + config_.name()), description) != 0) {
        std::cerr << "[ERROR] Failed to create a backup :/\n";
        return 1;
    }
    std::cout << "Successfully created a backup.\n";
    return 0;
}
