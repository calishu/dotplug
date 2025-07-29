#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "settings.hpp"
#include "utils/colors.hpp"
#include "utils/logging.hpp"

namespace fs = std::filesystem;

Logging::Logging(const LoggingLevel &level, const nlohmann::json &locale, const bool log_to_file)
    : level_(level),
      locale_(locale) {
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
        std::cout << colors::light_black << "Log file is located at: " << log_file_path_ << "\n" << colors::reset;
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
        throw std::logic_error(
            "You can't log a 'LoggingLevel::PROMPT', for that use function 'LoggingLevel::prompt'\n");

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
