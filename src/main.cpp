#include "CLI11.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  CLI::App app{"Dotplug is a easy-toruse dotfile manager for linux."};
  argv = app.ensure_utf8(argv);

  std::string name;
  bool apply = false, remove = false, create_new = false;

  auto apply_opt = app.add_flag("-a,--apply", apply, "Apply a dotfile configuration.");
  auto remove_opt = app.add_flag("-r,--remove", remove, "Remove a dotfile configuration.");
  auto new_opt = app.add_flag("-n,--new", create_new, "Creates a new configuration.");

  auto name_opt = app.add_option("name", name, "The name of your configuration.")
    ->check([](const std::string& value) -> std::string {
      return value.empty()  ? "Name cannot be empty" : "";
    });

  apply_opt->needs(name_opt);
  remove_opt->needs(name_opt);
  new_opt->needs(name_opt);

  CLI11_PARSE(app, argc, argv);

  
  if (apply)
    std::cout << "Apply config: " << name << std::endl;
  else if (remove)
    std::cout << "Remove config: " << name << std::endl;
  else if (create_new)
    std::cout << "Create config: " << name << std::endl;
  else
    std::cout << "No Action provided.";


  return 0;
}
