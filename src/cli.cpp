#include "include/cli.hpp"
#include "commands.hpp"

namespace cli {

auto setup(CLI::App *app, cli::Options *options) -> void {
    app->add_flag_callback("-v,--version", std::function<void()>(commands::version));
}

}
