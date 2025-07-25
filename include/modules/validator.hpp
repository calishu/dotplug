#pragma once

#include "utils/config.hpp"

#include <string>
#include <vector>

struct ValidationResult {
    enum class Type { Warning, Error };

    struct Entries {
        std::string message;
        Type type;
    };

    std::vector<Entries> log;

    inline bool has_errors() const {
        for (const auto &e : log)
            if (e.type == Type::Error)
                return true;
        return false;
    }

    inline bool has_warnings() const {
        for (const auto &e : log)
            if (e.type == Type::Warning)
                return true;
        return false;
    }

    void add_error(std::string msg);
    void add_warning(std::string msg);
};

void print_validation(ValidationResult &result);
ValidationResult validator(const Config &config);
