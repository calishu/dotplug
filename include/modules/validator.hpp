#pragma once

#include <string>
#include <vector>

#include "utils/logging.hpp"

class ValidationResult {
public:
    struct LogEntry {
        enum class Type { Warning, Error };

        std::string message;
        LoggingLevel level_;

        // explicit constructor needed for usage with std::vector::emplace_back
        LogEntry(const std::string &msg, const LoggingLevel level)
            : message{msg},
              level_{level} {}
    };

private:
    std::vector<LogEntry> log;

public:
    auto has_errors() const -> bool;
    auto has_warnings() const -> bool;
    auto add_error(const std::string &msg) -> void;
    auto add_warning(const std::string &msg) -> void;
    auto print() const -> void;
};
