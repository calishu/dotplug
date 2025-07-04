#include "CLI11.hpp"
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
  bool forced = false;
  
  auto list_cmd = app.add_subcommand("list", "Shows a list of all installed configurations.");

  auto install_cmd = app.add_subcommand("install", "Install a dotfile configuration from remote repository.");
  install_cmd->add_option("url", value, "The Git URL of the remote repository.")->required();

  auto apply_cmd = app.add_subcommand("apply", "Apply a dotfile configuration.");
  apply_cmd->add_option("name", value, "The name of the dotfile configuration you want to apply.")->required();

  auto remove_cmd = app.add_subcommand("remove", "Remove a dotfile configuration.");
  remove_cmd->add_option("name", value, "The name of the dotfile configuration you want to remove.")->required();

  auto new_cmd = app.add_subcommand("new", "Creates a new dotfile configuration.");
  new_cmd->add_option("name", value, "The name of the dotfile configuration you want to create.")->required();

  app.add_flag("-f,--force", forced, "Force the action and ignore every possible warning. (CAUTION!)");


  CLI11_PARSE(app, argc, argv);

  if (forced) {
    std::string proceed;
    std::cout << "Are you sure you want to continue and force the actions? (y/N) ";
    std::getline(std::cin, proceed);

    if(proceed == "N" || proceed == "n" || proceed.empty()) return 0;
  }

  
  // actions
  if (&list_cmd) list();

  return 0;
}
