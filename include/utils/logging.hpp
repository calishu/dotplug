#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "utils/colors.hpp"

enum class LoggingLevel { DEBUG, INFO, WARNING, ERROR, PROMPT };
enum class PromptMode { BOOL, STRING, INTEGER };

class Logging {
    LoggingLevel level_;
    nlohmann::json lang_;
    std::fstream log_file_;
    bool file_logging_enabled_ = false;
    std::string log_file_path_;
    std::ostringstream log_stream_;

public:
    Logging(const LoggingLevel &level, const nlohmann::json &locale, const bool log_to_file = false);
    ~Logging();

    auto
    log(const LoggingLevel &level,
        const std::string &message,
        const std::string &note   = "",
        const std::string &prefix = "",
        const std::string &suffix = "") -> void;

    auto prompt(const PromptMode &mode, const std::string &prompt, const std::string &specific) -> std::string;

    inline auto level_to_string(LoggingLevel level) -> std::string {
        // clang-format off
        switch (level) {
            case LoggingLevel::DEBUG:
                return lang_["logging"]["types"]["debug"];
            case LoggingLevel::INFO:
                return lang_["logging"]["types"]["info"];
            case LoggingLevel::WARNING:
                return lang_["logging"]["types"]["warning"];
            case LoggingLevel::ERROR:
                return lang_["logging"]["types"]["error"];
            case LoggingLevel::PROMPT:
                return lang_["logging"]["types"]["prompt"];
            default:
                throw std::invalid_argument(lang_["logging"]["errors"]["invalid_level"]);
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
                return colors::cyan;
            default:
                throw std::invalid_argument(lang_["logging"]["errors"]["invalid_level"]);

        }
        // clang-format on
    }
};
