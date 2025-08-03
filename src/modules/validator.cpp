#include <iostream>
#include <string>
#include <vector>

#include "context.hpp"
#include "modules/validator.hpp"
#include "utils/logging.hpp"

auto ValidationResult::has_errors() const -> bool {
    for (const auto &e : log)
        if (e.level_ == LoggingLevel::ERROR)
            return true;
    return false;
}

auto ValidationResult::has_warnings() const -> bool {
    for (const auto &e : log)
        if (e.level_ == LoggingLevel::WARNING)
            return true;
    return false;
}

auto ValidationResult::add_error(const std::string &msg) -> void {
    log.emplace_back(msg, LoggingLevel::ERROR);
}

auto ValidationResult::add_warning(const std::string &msg) -> void {
    log.emplace_back(msg, LoggingLevel::WARNING);
}

auto ValidationResult::print() const -> void {
    if (log.empty()) {
        ctx->logging->log(LoggingLevel::INFO, ctx->locale.json["validation"]["statements"]["success"]);
        return;
    }

    for (const auto &e : log)
        if (e.level_ == LoggingLevel::ERROR)
            ctx->logging->log(LoggingLevel::ERROR, e.message);
        else if (e.level_ == LoggingLevel::WARNING)
            ctx->logging->log(LoggingLevel::WARNING, e.message);
}
