#pragma once

#include "nlohmann/json.hpp"
#include <locale>
#include <utility>

struct locale_data {
    nlohmann::json json;
    std::locale locale;
};

auto get_locale() -> locale_data;
