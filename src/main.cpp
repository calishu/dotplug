#include <CLI/CLI.hpp>
#include <git2.h>
#include <iostream>
#include <string>
#include <unistd.h>

#include "context.hpp"
#include "modules/apply.hpp"
#include "modules/install.hpp"
#include "modules/list.hpp"
#include "modules/new_config.hpp"
#include "modules/remove.hpp"
#include "modules/validator.hpp"
#include "utils/config.hpp"
#include "utils/symlinks.hpp"

int main(int argc, char **argv) {
    if (!getuid()) {
        std::cerr << "Using `sudo` is prohibited.\n";
        return EXIT_FAILURE;
    }

    CLI::App app{"Dotplug is a easy-to-use dotfile manager for linux."};
    argv = app.ensure_utf8(argv);
    app.require_subcommand();

    std::string value;     // stuff like names or links.
    std::string snd_value; // second value
    auto dependencies = std::vector<std::string>{};
    bool forced       = false;

    auto add_force_flag = [&](CLI::App *cmd) {
        cmd->add_flag("-f,--force", forced, "Force the action and ignore warnings (CAUTION!)");
    };

    // auto add_flag = [&](CLI::App* cmd, const std::string& name, const
    // std::string& description) {
    //   cmd->add_flag(name, description);
    // };

    auto list_cmd = app.add_subcommand("list", "Shows a list of all installed configurations.");

    auto install_cmd = app.add_subcommand("install", "Install a dotfile configuration from remote repository.");
    install_cmd->add_option("url", value, "The Git URL of the remote repository.")->required();

    auto init_cmd = app.add_subcommand("init", "Initializes a new dotfile configuration.");
    init_cmd->add_option("name", value, "The name of the dotfile configuration you want to initialize.")->required();
    init_cmd->add_option("-d,--dependencies", dependencies, "List of the dependencies of the config you add.")
        ->expected(-1)
        ->required();

    auto config_cmd = app.add_subcommand("config", "Do stuff with your configuration.");
    config_cmd->add_option("name", value, "The name of the configuration.")->required();

    auto validate_cmd = config_cmd->add_subcommand("validate", "Check if the configuration config is valid."),
         disable_cmd  = config_cmd->add_subcommand("disable", "Disable the current configuration."),
         remove_cmd   = config_cmd->add_subcommand("remove", "Remove a dotfile configuration."),
         apply_cmd    = config_cmd->add_subcommand("apply", "Apply a dotfile configuration."),
         show_cmd     = config_cmd->add_subcommand("show", "Shows a specific configuration.");

    auto backup_cmd = app.add_subcommand("backup", "Manages the backups of your configuration files.");
    backup_cmd->add_option("name", value, "The name of the configuration.")->required();

    auto b_restore_cmd = backup_cmd->add_subcommand("restore", "Rolls back to the initial state, before dotplug."),
         b_create_cmd  = backup_cmd->add_subcommand("create", "Create a backup of the configuration."),
         b_apply_cmd   = backup_cmd->add_subcommand("apply", "Apply a backup of the configuration."),
         b_list_cmd    = backup_cmd->add_subcommand("list", "Gives you a list of all backups on this configuration.");

    b_apply_cmd->add_option("id", snd_value, "What is the backup id?")->required();
    b_create_cmd->add_option("description", snd_value, "What is the description of the backup?");

    // auto disable_cmd = app.add_subcommand("disable", "Disables your current
    // configuration.");

    add_force_flag(remove_cmd);
    // add_flag(remove_cmd, "-f,--force", forced, "Force the action and ignore
    // warnings (CAUTION!)");

    CLI11_PARSE(app, argc, argv);

    git_libgit2_init();

    if (forced) {
        std::string proceed;
        std::cout << "Are you sure you want to continue and force the actions? "
                     "(y/N) ";
        std::getline(std::cin, proceed);

        if (proceed == "N" || proceed == "n" || proceed.empty())
            return 0;
    }
    ctx->forced = forced;

    if (!value.empty())
        ctx->name = value;

    try {
        // actions
        if (list_cmd->parsed())
            list();
        else if (init_cmd->parsed())
            new_config(dependencies);
        else if (install_cmd->parsed())
            install();

        else if (config_cmd->parsed()) {
            if (validate_cmd->parsed())
                Config{value}.validate().print();
            else if (remove_cmd->parsed())
                remove_config();
            else if (show_cmd->parsed())
                list();
            else if (apply_cmd->parsed())
                apply();
            else if (disable_cmd->parsed())
                remove_all_symlinks();
            else
                list();
        }
    } catch (const std::exception &e) {
        std::cerr << argv[0] << ": " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    git_libgit2_shutdown();
}
