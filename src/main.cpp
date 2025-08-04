#include <CLI/CLI.hpp>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <string>
#include <unistd.h>

#include "context.hpp"
#include "modules/apply.hpp"
#include "modules/install.hpp"
#include "modules/list.hpp"
#include "modules/new_config.hpp"
#include "modules/remove.hpp"
#include "modules/validator.hpp"
#include "utils/colors.hpp"
#include "utils/config.hpp"
#include "utils/lang.hpp"
#include "utils/logging.hpp"
#include "utils/symlinks.hpp"

struct init_opts_ {
    std::string name;
    std::vector<std::string> dependencies;
};

int main(int argc, char **argv) {
    auto locale = get_locale();
    auto lang   = locale.json;

    auto logging = Logging{LoggingLevel::DEBUG, locale, false};

    // check if `sudo` or in root
    if (!getuid()) {
        logging.log(LoggingLevel::ERROR, lang["cli"]["errors"]["root_prohibited"]);
        return EXIT_FAILURE;
    }

    CLI::App app{lang["cli"]["description"]};

    argv = app.ensure_utf8(argv);
    app.require_subcommand();

    bool forced = false;

    auto add_force_flag = [&](CLI::App *cmd) {
        cmd->add_flag("-f,--force", forced, lang["cli"]["commands"]["force"]);
    };

    /// `dotplug list`
    auto list_cmd = app.add_subcommand("list", lang["cli"]["commands"]["list"]);

    /// `dotplug disable`
    // std::string disable_url;
    auto disable_cmd = app.add_subcommand("disable", lang["cli"]["commands"]["disable"]["desc"]);
    // disable_cmd->add_option("name", disable_url, lang["cli"]["commands"]["disable"]["desc"]);

    /// `dotplug install <url>`
    std::string install_url;
    auto install_cmd = app.add_subcommand("install", lang["cli"]["commands"]["install"]["desc"]);
    install_cmd->add_option("url", install_url, lang["cli"]["commands"]["install"]["url"]);

    /// `dotplug init <name> (optional)<dependencies>`
    init_opts_ init_opts;
    auto init_cmd = app.add_subcommand("init", lang["cli"]["commands"]["init"]["desc"]);
    init_cmd->add_option("name", init_opts.name, lang["cli"]["commands"]["init"]["name"])->required();
    init_cmd->add_option("-d,--dependencies", init_opts.dependencies, lang["cli"]["commands"]["init"]["dependencies"])
        ->allow_extra_args(true);

    /// `dotplug config <name> <action>`
    std::string config_name;
    auto config_cmd = app.add_subcommand("config", lang["cli"]["commands"]["config"]["desc"]);
    config_cmd->add_option("name", config_name, lang["cli"]["commands"]["config"]["name"]);

    // clang-format off
    auto validate_action = config_cmd->add_subcommand("validate",lang["cli"]["commands"]["config"]["actions"]["validate"]), 
         delete_action   = config_cmd->add_subcommand("delete", lang["cli"]["commands"]["config"]["actions"]["delete"]),
         apply_action    = config_cmd->add_subcommand("apply", lang["cli"]["commands"]["config"]["actions"]["apply"]),
         show_action     = config_cmd->add_subcommand("show", lang["cli"]["commands"]["config"]["actions"]["show"]);
    // clang-format on

    add_force_flag(delete_action);

    CLI11_PARSE(app, argc, argv);

    if (forced && !std::get<bool>(logging.prompt(PromptMode::BOOL, lang["cli"]["prompts"]["force_confirm"], "1")))
        return 0;

    // add context values (reference: src/context.hpp, include/context.hpp)
    ctx->forced  = forced;
    ctx->logging = &logging;
    ctx->locale  = locale;

    try {
        if (list_cmd->parsed())
            list();
        else if (init_cmd->parsed())
            new_config(init_opts.name, init_opts.dependencies);
        else if (install_cmd->parsed()) {
            ctx->name = install_url;
            install();
        } else if (disable_cmd->parsed())
            remove_all_symlinks();
        else if (config_cmd->parsed()) {
            ctx->name = config_name;

            if (validate_action->parsed())
                Config{config_name}.validate().print();
            else if (show_action->parsed())
                list();
            else if (apply_action->parsed())
                apply();
            else if (delete_action->parsed())
                remove_config();
            else
                list();
        }
    } catch (const std::exception &e) {
        logging.log(LoggingLevel::ERROR, e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
