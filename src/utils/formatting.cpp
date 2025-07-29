#include <algorithm>
#include <regex>
#include <string>

#include "context.hpp"

// format checks
bool is_valid_url() {
    const auto pos = ctx->name.find("://");
    return (pos != std::string::npos) && (pos < 10);
}

// formatting
auto strip_ansi(const std::string &str) -> std::string {
    static const std::regex ansi_regex("\x1B\\[[0-9;]*[a-zA-Z]");
    return std::regex_replace(str, ansi_regex, "");
}

auto string_lower(const std::string &string, const std::locale &locale) -> std::string {
    std::string result = string;

    std::transform(
        result.begin(), result.end(), result.begin(), [&locale](unsigned char c) { return std::tolower(c, locale); });

    return result;
}

