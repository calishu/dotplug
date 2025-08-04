#include <iostream>
#include <sstream>
#include <string>

#include "context.hpp"
#include "modules/install.hpp"
#include "modules/validator.hpp"
#include "settings.hpp"
#include "utils/config.hpp"
#include "utils/formatting.hpp"

int install() {
    auto locale   = ctx->locale;
    auto *logging = ctx->logging;

    if (!is_valid_url()) {
        logging->log(LoggingLevel::ERROR, locale.json["install"]["errors"]["invalid_url"]);
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
    if (!git)
        throw std::runtime_error{std::string{"popen: "} + strerror(errno)};

    // pclose() waits for the process to exit
    // all we need is a success/failure, so we can check the exit status for that
    const auto rc = pclose(git);
    if (rc == -1)
        throw std::runtime_error{std::string{"pclose: "} + strerror(errno)};

    const auto status = WEXITSTATUS(rc);
    logging->log(
        LoggingLevel::DEBUG, std::string(locale.json["install"]["statements"]["status"]) + std::to_string(status));
    if (status)
        throw std::runtime_error{locale.json["install"]["errors"]["clone_failed"]};

    logging->log(LoggingLevel::INFO, locale.json["install"]["statements"]["success"]);
    if (!std::get<bool>(logging->prompt(PromptMode::BOOL, locale.json["install"]["prompts"]["validation"], "1")))
        return 0;

    const Config config{name};
    const auto validation_result = config.validate();

    if (validation_result.has_errors())
        validation_result.print();

    return 0;
}
