#pragma once

#include <string>
#include <vector>

struct ValidationResult {
    enum class Type { Warning, Error };

    struct Entries {
        std::string message;
        Type type;
    };

    std::vector<Entries> log;

    auto has_errors() const -> bool;
    auto has_warnings() const -> bool;
    auto add_error(const std::string &msg) -> void;
    auto add_warning(const std::string &msg) -> void;
    auto print() const -> void;
};
