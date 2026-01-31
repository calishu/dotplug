#include "include/logging.hpp"

auto Logging::LogLevel::print(std::ostream& os) const -> std::ostream & {
    if (!isatty(STDOUT_FILENO) || std::getenv("NO_COLOR"))
        return os << "[ " << name << " ] ";
    return os << color_code << "[ " << name << " ]\033[0m ";
}


auto Logging::log(const Logging::LogLevel &level, std::string_view message) -> void {
    std::ostringstream stream;
    stream << "\033[0;0m" << "[";

    const auto now = std::chrono::system_clock::now();
    const auto now_ms = std::chrono::floor<std::chrono::milliseconds>(now);

    stream << std::format("{:%T}", now_ms) << "]\033[0m ";
    level.print(stream);
    stream << message << "\n";
    std::cout << stream.str();
}
