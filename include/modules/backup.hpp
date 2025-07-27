#pragma once

#include <git2/repository.h>

#include "utils/config.hpp"

class Backup {
    Config config_;
    std::string backup_path_;

public:
    Backup(const Config &config);

    auto create(const std::string &description = "") const -> int;
};
