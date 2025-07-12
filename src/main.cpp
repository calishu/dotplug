#include "new_config.hpp"
#include "validator.hpp"
#include "settings.hpp"
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

  auto validate_cmd = app.add_subcommand("validate", "Check if the configuration config is valid.");
  validate_cmd->add_option("name", value, "The name of the dotfile configuration you want to check.")->required();

  auto install_cmd = app.add_subcommand("install", "Install a dotfile configuration from remote repository.");
  install_cmd->add_option("url", value, "The Git URL of the remote repository.")->required();

  auto apply_cmd = app.add_subcommand("apply", "Apply a dotfile configuration.");
  apply_cmd->add_option("name", value, "The name of the dotfile configuration you want to apply.")->required();

  auto remove_cmd = app.add_subcommand("remove", "Remove a dotfile configuration.");
  remove_cmd->add_option("name", value, "The name of the dotfile configuration you want to remove.")->required();

  auto new_cmd = app.add_subcommand("new", "Creates a new dotfile configuration.");
  new_cmd->add_option("name", value, "The name of the dotfile configuration you want to create.")->required();
  new_cmd->add_option("-d,--dependencies", dependencies, "List of the dependencies of the config you add.")->expected(-1)->required();

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
  else if (validate_cmd->parsed()) {
    auto parsed = parse_config(value);
    if (parsed.second == 1) {
      return 1;
    }
    ValidationResult validation_result = validator(parsed.first);
    print_validation(validation_result);
  }
  else if (new_cmd->parsed()) new_config(dependencies);
  else if (remove_cmd->parsed()) remove_config();

  return 0;
}
