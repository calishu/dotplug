#include "CLI/CLI.hpp"
#include "include/cli.hpp"

int main(int argc, char** argv) {
    CLI::App app{"dotplug"};

    cli::Options config;
    cli::setup(&app, &config);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    return 0;
}
