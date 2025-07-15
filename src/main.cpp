#include "new_config.hpp"
#include "validator.hpp"
#include "settings.hpp"
#include "symlinks.hpp"
#include "install.hpp"
#include "context.hpp"
#include "remove.hpp"
#include "config.hpp"
#include "CLI/CLI.hpp"
#include "list.hpp"
#include <iostream>
#include <unistd.h>
#include <string>

int main(int argc, char** argv) {
  if (!getuid()) {
    std::cout << "Using `sudo` is prohibited." << std::endl;
    return 1;
  }

  CLI::App app{"Dotplug is a easy-to-use dotfile manager for linux."};
  argv = app.ensure_utf8(argv);
  app.require_subcommand();

  std::string value; // stuff like names or links.
  std::vector<std::string> dependencies;
  bool forced = false;

  auto add_force_flag = [&](CLI::App* cmd) {
    cmd->add_flag("-f,--force", forced, "Force the action and ignore warnings (CAUTION!)");
  };
  
  auto list_cmd = app.add_subcommand("list", "Shows a list of all installed configurations.");

  auto install_cmd = app.add_subcommand("install", "Install a dotfile configuration from remote repository.");
  install_cmd->add_option("url", value, "The Git URL of the remote repository.")->required();

  auto init_cmd = app.add_subcommand("init", "Initializes a new dotfile configuration.");
  init_cmd->add_option("name", value, "The name of the dotfile configuration you want to initialize.")->required();
  init_cmd->add_option("-d,--dependencies", dependencies, "List of the dependencies of the config you add.")->expected(-1)->required();


  auto config_cmd = app.add_subcommand("config", "Show/Edit your configuration.");
  config_cmd->add_option("name", value, "The name of the configuration you want to show or edit.")->required();

  auto validate_cmd = config_cmd->add_subcommand("validate", "Check if the configuration config is valid.");
  auto remove_cmd = config_cmd->add_subcommand("remove", "Remove a dotfile configuration.");
  auto apply_cmd = config_cmd->add_subcommand("apply", "Apply a dotfile configuration.");
  auto show_cmd = config_cmd->add_subcommand("show", "Shows a specific configuration.");


  // auto disable_cmd = app.add_subcommand("disable", "Disables your current configuration.");

  add_force_flag(remove_cmd);

  CLI11_PARSE(app, argc, argv);

  if (forced) {
    std::string proceed;
    std::cout << "Are you sure you want to continue and force the actions? (y/N) ";
    std::getline(std::cin, proceed);

    if(proceed == "N" || proceed == "n" || proceed.empty()) return 0;
  }

  if (!value.empty())
    ctx->name = value;
  if (forced)
    ctx->forced = forced;
  
  // actions
  if (list_cmd->parsed()) list();
  else if (init_cmd->parsed()) new_config(dependencies);
  else if (install_cmd->parsed()) install();
 
  else if (config_cmd->parsed()) {
    if (validate_cmd->parsed()) {
      auto parsed = parse_config(value);
      if (parsed.second == 1) {
        return 1;
      }
      ValidationResult validation_result = validator(parsed.first);
      print_validation(validation_result);
    }
    else if (remove_cmd->parsed()) remove_config();
    else if (show_cmd->parsed()) list();
    else if (apply_cmd->parsed()) std::cout << "This is just a placeholder owo" << std::endl;
    else list();
  }

  return 0;
}
