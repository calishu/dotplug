#include <string>

#include "utils/colors.hpp"
#include "utils/logging.hpp"

Logging::Logging(const LoggingLevel &level, const nlohmann::json &locale)
    : level_(level),
      locale_(locale) {};

auto Logging::log(const LoggingLevel &level, const std::string &message, const std::string &note) -> void {
    if (level < level_)
        return;

    std::cout << colors::light_black << "(" << colors::bold << level_get_color(level) << level_to_string(level)
              << colors::reset << colors::light_black << ") " << colors::reset << message << "\n";

    if (!note.empty())
        std::cout << colors::bold << colors::bg_blue << "NOTE:" << colors::reset << " " << note << "\n";
}
