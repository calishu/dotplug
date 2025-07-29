#include <string>

#include "utils/colors.hpp"
#include "utils/logging.hpp"

Logging::Logging(const LoggingLevel &level, const nlohmann::json &locale)
    : level_(level),
      locale_(locale) {};

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
}
