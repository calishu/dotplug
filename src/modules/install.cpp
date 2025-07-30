#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

#include "context.hpp"
#include "modules/install.hpp"
#include "modules/validator.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/formatting.hpp"

int install() {
    if (!is_valid_url()) {
        std::cerr << "The provided value is not a URL!\n";
        return 1;
    }

    const auto last_slash = ctx->name.find_last_of("/");
    if (last_slash == std::string::npos)
        return 1;

    auto name = ctx->name.substr(last_slash + 1);

    if (name.size() >= 4 && name.compare(name.size() - 4, 4, ".git") == 0)
        name.erase(name.size() - 4);

    const auto clone_path = dotfiles_path + name;
    std::ostringstream ss;

    // always quote strings in a popen call because popen uses the system shell
    ss << "git clone '" << ctx->name << "' '" << clone_path << '\'';
    
    const auto git = popen(ss.str().c_str(), "r");
    if (!git) {
        throw std::runtime_error{std::string{"popen: "} + strerror(errno)};
    }

    // pclose() waits for the process to exit
    // all we need is a success/failure, so we can check the exit status for that
    const auto rc = pclose(git);
    if (rc == -1) {
        throw std::runtime_error{std::string{"pclose: "} + strerror(errno)};
    }

    const auto status = WEXITSTATUS(rc);
    std::cout << "git exited with status: " << status << '\n';
    if (status) {
        throw std::runtime_error{"git clone failed!"};
    }

    std::cout << "Downloaded the config! Wanna validate it? (Y/n) ";

    std::string validate_choice;
    std::getline(std::cin, validate_choice);
    if (validate_choice.empty() || validate_choice == "n" || validate_choice == "N")
        return 0;

    const Config config{name};
    const auto validation_result = config.validate();

    if (validation_result.has_errors())
        validation_result.print();

    return 0;
}
