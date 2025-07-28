#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "utils/colors.hpp"

enum class LoggingLevel { DEBUG, INFO, WARNING, ERROR, PROMPT };

class Logging {
    LoggingLevel level_;
    nlohmann::json locale_;

public:
    Logging(const LoggingLevel &level, const nlohmann::json &locale);

    auto log(const LoggingLevel &level, const std::string &message, const std::string &note = "") -> void;

    inline auto level_to_string(LoggingLevel level) -> std::string {
        // clang-format off
        switch (level) {
            case LoggingLevel::DEBUG:
                return locale_["logging"]["types"]["debug"];
            case LoggingLevel::INFO:
                return locale_["logging"]["types"]["info"];
            case LoggingLevel::WARNING:
                return locale_["logging"]["types"]["warning"];
            case LoggingLevel::ERROR:
                return locale_["logging"]["types"]["error"];
            case LoggingLevel::PROMPT:
                return locale_["logging"]["types"]["prompt"];
            default:
                throw std::invalid_argument(locale_["logging"]["errors"]["invalid_level"]);
        }
        // clang-format on
    }

    inline auto level_get_color(LoggingLevel level) {
        // clang-format off
        switch (level) {
            case LoggingLevel::DEBUG:
                return colors::light_black;
            case LoggingLevel::INFO:
                return colors::cyan;
            case LoggingLevel::WARNING:
                return colors::yellow;
            case LoggingLevel::ERROR:
                return colors::light_red;
            case LoggingLevel::PROMPT:
                return colors::light_cyan;
            default:
                throw std::invalid_argument(locale_["logging"]["errors"]["invalid_level"]);

        }
        // clang-format on
    }
};
