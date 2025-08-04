#include <algorithm>
#include <locale>
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

    const auto &facet = std::use_facet<std::ctype<char>>(locale);

    std::transform(
        result.begin(), result.end(), result.begin(), [&facet](unsigned char c) { return facet.tolower(c); });

    return result;
}

// deletes first whitespace if exists
auto trim(const std::string &str) -> std::string {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// replaces {} with provided content
auto replace_format(std::string content, const std::string &replace) -> std::string {
    size_t pos = content.find("{}");
    if (pos != std::string::npos)
        content.replace(pos, std::string("{}").length(), replace);

    return content;
}

auto resolve_path(const std::string &path) -> std::string {
    if (path[0] != '~')
        return path;
    return std::getenv("HOME") + path.substr(1);
}
