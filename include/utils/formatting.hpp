#pragma once

#include <locale>
#include <string>

bool is_valid_url();

auto strip_ansi(const std::string &str) -> std::string;
auto string_lower(const std::string &string, const std::locale &locale) -> std::string;
auto trim(const std::string &str) -> std::string;
auto replace_format(std::string content, const std::string &replace) -> std::string;
