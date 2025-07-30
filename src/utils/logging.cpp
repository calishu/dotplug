#include <chrono>
#include <filesystem>
#include <fstream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>

#include "settings.hpp"
#include "utils/colors.hpp"
#include "utils/formatting.hpp"
#include "utils/logging.hpp"

namespace fs = std::filesystem;

Logging::Logging(const LoggingLevel &level, const nlohmann::json &lang, const bool log_to_file)
    : level_(level),
      lang_(lang) {
    if (!log_to_file)
        return;

    std::ostringstream filename;
    auto now  = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    fs::create_directory(share_path + "logs/");
    filename << share_path + "logs/log_" << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S") << ".log";

    log_file_.open(filename.str(), std::ios::out);
    if (!log_file_)
        throw std::runtime_error("Couldn't open '" + filename.str() + "'\n");
    file_logging_enabled_ = log_file_.is_open();
    log_file_path_        = filename.str();
};

Logging::~Logging() {
    if (file_logging_enabled_) {
        std::cout << colors::light_black << lang_["logging"]["statements"]["file_location"] << log_file_path_ << "\n"
                  << colors::reset;
        log_file_ << strip_ansi(log_stream_.str());
    }

    log_file_.close();
};

auto Logging::log(
    const LoggingLevel &level,
    const std::string &message,
    const std::string &note,
    const std::string &prefix,
    const std::string &suffix) -> void {
    if (level == LoggingLevel::PROMPT)
        throw std::logic_error(lang_["logging"]["errors"]["no_prompt_log"]);

    if (level < level_)
        return;

    std::ostringstream log_stream;

    log_stream << colors::light_black << "(" << colors::bold << level_get_color(level) << level_to_string(level)
               << colors::reset << colors::light_black << ") " << colors::reset;

    if (!prefix.empty())
        log_stream << colors::white << prefix << " : " << colors::reset;

    log_stream << message;

    if (!suffix.empty())
        log_stream << colors::light_black << " (" << suffix << ")" << colors::reset;

    log_stream << "\n";

    if (!note.empty())
        log_stream << colors::bold << colors::bg_blue << "NOTE:" << colors::reset << " " << note << "\n";

    std::cout << log_stream.str();

    if (file_logging_enabled_)
        log_stream_ << log_stream.str();
}

// specific is the value checked in a specific prompt mode.
auto Logging::prompt(const PromptMode &mode, const std::string &prompt, const std::string &specific) -> std::string {
beginning:
    const auto locale         = std::locale();
    const auto lower_specific = string_lower(specific, locale);

    std::ostringstream log_stream;

    log_stream << colors::light_black << "(" << colors::bold << level_get_color(LoggingLevel::PROMPT)
               << level_to_string(LoggingLevel::PROMPT) << colors::reset << colors::light_black << ") " << colors::reset
               << prompt << " ";

    // prefix, e.g. number range, defaults, etc.
    switch (mode) {
    case PromptMode::BOOL:
        if (lower_specific != "0" && lower_specific != "1")
            throw std::logic_error(lang_["logging"]["errors"]["specific_bool_needs_bool"]);
        log_stream << "(" << (lower_specific == "0" ? "Y" : "y") << "/" << (lower_specific == "0" ? "n" : "N") << ")";

    default:
        break;
    }

    log_stream << ": ";
    std::cout << log_stream.str();

    std::string user_input;
    std::getline(std::cin, user_input);

    std::string lower_user_input = string_lower(user_input, locale);

    switch (mode) {
    // checks for the modes
    case PromptMode::BOOL:
        if (lower_user_input == "y")
            return "true";
        else if (lower_user_input == "n")
            return "false";
        else if (lower_user_input.empty())
            return (lower_specific == "0" ? "true" : "false");
        else {
            log(LoggingLevel::ERROR, lang_["logging"]["errors"]["invalid_input_choice"]);
            goto beginning;
        }

    case PromptMode::STRING:
        if (specific.empty()) {
            log(LoggingLevel::ERROR, lang_["logging"]["errors"]["string_input_empty"]);
            goto beginning;
        }

    case PromptMode::INTEGER:
        try {
            std::stoi(user_input);
        } catch (std::exception &e) {
            log(LoggingLevel::ERROR, lang_["logging"]["error"]["input_not_integer"]);
            goto beginning;
        }

    default:
        break;
    }

    return lower_user_input;
}
