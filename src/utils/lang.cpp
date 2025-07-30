#include <exception>
#include <fstream>
#include <locale>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

#include "utils/lang.hpp"

auto get_locale() -> locale_data {
    locale_data locale;

    std::locale loc("");
    std::string name = loc.name().substr(0, loc.name().find("."));

    nlohmann::json parsed;

    try {
        std::ifstream lang("lang/" + name + ".json");
        if (!lang.is_open())
            throw std::runtime_error("Lang file couldn't be found.");
        lang >> parsed;
    } catch (std::exception &e) {
        std::ifstream fallback("lang/en_US.json");
        if (!fallback.is_open())
            throw std::runtime_error(
                "Something went definitely wrong, please reinstall dotplug or restore the 'lang/en_US.json' file.");
        fallback >> parsed;
    }

    locale.locale = loc;
    locale.json   = parsed;

    return locale;
}
