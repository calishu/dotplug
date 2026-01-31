#pragma once

#include <CLI/CLI.hpp>
#include <string>

namespace cli {

struct Options {
    std::string input;
    std::string output;
    int verbose = 0; // 0 = false, 1 = true
};

auto setup(CLI::App *app, cli::Options *options) -> void;

}
