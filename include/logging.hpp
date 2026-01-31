#include <string>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>

class Logging {
    struct LogLevel {
        std::string_view name;
        std::string_view color_code; // ansi

        auto print(std::ostream& os) const -> std::ostream&;
    };

public:
    static constexpr LogLevel debug = { .name = "Debug", .color_code = "\033[1;90m" };
    static constexpr LogLevel info = { .name = "Info", .color_code = "\033[1;96m" };
    static constexpr LogLevel success = { .name = "Success", .color_code = "\033[1;92m" };
    static constexpr LogLevel warning = { .name = "Warning", .color_code = "\033[1;93m" };
    static constexpr LogLevel error = { .name = "Error", .color_code = "\033[1;91m" };
    static constexpr LogLevel fatal = { .name = "Fatal", .color_code = "\033[1;31m" };

    auto log(const Logging::LogLevel &level, std::string_view message) -> void;
};
