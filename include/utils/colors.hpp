#pragma once
#include <iostream>

namespace color {

inline bool is_color_enabled() {
    static bool enabled = (std::getenv("NO_COLOR") == nullptr);
    return enabled;
}

// "state" colors (found no better name)
inline std::ostream &reset(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[0m";
    return os;
}

inline std::ostream &bold(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[1m";
    return os;
}
inline std::ostream &underline(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[4m";
    return os;
}

// normal colors
inline std::ostream &black(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[30m";
    return os;
}
inline std::ostream &red(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[31m";
    return os;
}
inline std::ostream &green(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[32m";
    return os;
}
inline std::ostream &yellow(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[33m";
    return os;
}
inline std::ostream &blue(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[34m";
    return os;
}
inline std::ostream &magenta(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[35m";
    return os;
}
inline std::ostream &cyan(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[36m";
    return os;
}
inline std::ostream &white(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[37m";
    return os;
}

// light colors
inline std::ostream &light_black(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[90m";
    return os;
}
inline std::ostream &light_red(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[91m";
    return os;
}
inline std::ostream &light_green(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[92m";
    return os;
}
inline std::ostream &light_yellow(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[93m";
    return os;
}
inline std::ostream &light_blue(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[94m";
    return os;
}
inline std::ostream &light_magenta(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[95m";
    return os;
}
inline std::ostream &light_cyan(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[96m";
    return os;
}
inline std::ostream &light_white(std::ostream &os) {
    if (is_color_enabled())
        os << "\033[97m";
    return os;
}

} // namespace color
