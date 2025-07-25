#pragma once

#include <string>
#include <vector>

class ValidationResult {
public:
    struct LogEntry {
        enum class Type { Warning, Error };

        std::string message;
        Type type;

        // explicit constructor needed for usage with std::vector::emplace_back
        LogEntry(const std::string &msg, const Type t)
            : message{msg},
              type{t} {}
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
