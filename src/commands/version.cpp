#include "include/commands/version.hpp"
#include "info.hpp"

auto commands::version() -> void {
    std::cout << VERSION << "\n";
}
