#pragma once

#include "utils/lang.hpp"
#include "utils/logging.hpp"
#include <string>

struct Context {
    std::string name;
    Logging *logging;
    locale_data locale;
    bool forced;
};

extern Context *ctx;
