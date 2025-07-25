#include <iostream>
#include <vector>

#include "modules/validator.hpp"

auto ValidationResult::has_errors() const -> bool {
    for (const auto &e : log)
        if (e.type == LogEntry::Type::Error)
            return true;
    return false;
}

auto ValidationResult::has_warnings() const -> bool {
    for (const auto &e : log)
        if (e.type == LogEntry::Type::Warning)
            return true;
    return false;
}

auto ValidationResult::add_error(const std::string &msg) -> void {
    log.emplace_back(msg, LogEntry::Type::Error);
}

auto ValidationResult::add_warning(const std::string &msg) -> void {
    log.emplace_back(msg, LogEntry::Type::Warning);
}

auto ValidationResult::print() const -> void {
    if (log.empty())
        std::cout << "Everything is fine with the configuration.\n";
    for (const auto &e : log)
        std::cout << (e.type == LogEntry::Type::Error ? "[ERROR] " : "[WARNING] ") << e.message << "\n";
    return;
}
