#include "config.hpp"
#include "context.hpp"
#include "settings.hpp"
#include "toml++/toml.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

/* @brief Removes every symlink from the active config!
 *
 * The `current` file in the base config path, contains the name of the active config.
 *
 * @return the return code 0 for success, 1 for fail/error.
 * */
int remove_all_symlinks() {
  std::string active_config_name;

  std::ifstream current(config_path + "current");
  if (current.good()) {
    std::getline(current, active_config_name);
  } else {
    std::cerr << "There is no active config!" << std::endl;
    return 1;
  }

  auto [active_config, parse_err] = parse_config(active_config_name);
  if (parse_err != 0) {
    std::cerr << "Something failed :(" << std::endl;
    return 1;
  }

  auto dependencies = active_config["dotplug"]["dependencies"];

  if (toml::array* arr = dependencies.as_array()) {
    if (arr->empty()) return 1;

    for (size_t i = 0; i < arr->size(); ++i) {
      std::string dep_name = (*arr)[i].value_or("");
      if (dep_name.empty()) {
        std::cout << dep_name << "\n";
        std::cerr << "[ERROR] Dependency name is empty!" << std::endl;
        continue;
      }

      const auto& dep_table_node = active_config["dotplug"][dep_name];

      if (auto* dep_table = dep_table_node.as_table()) {
        std::string destination = destination_path + (*dep_table)["destination"].value_or("");
        if (destination.empty()) {
          std::cout << destination << "\n";
          std::cerr << "[ERROR] Destination of " << dep_table_node << " is empty!" << std::endl;
          continue;
        }

        if (!std::filesystem::exists(destination)) {
          std::cerr << "[ERROR] Couldn't find " << destination << std::endl;
          continue;
        }
        if (!std::filesystem::is_symlink(destination)) {
          std::cout << "[WARNING] " << destination << "isn't a symlink! Want to delete it? (y/N)";
          std::string choice;
          std::getline(std::cin, choice);

          if (choice == "n" || choice == "N" || choice == "") continue;
        }

        std::error_code err;
        bool removed = std::filesystem::remove(destination, err);
        if (!removed || err) {
          std::cout << "Oh no, there was an error :<\n" << err.message() << std::endl;
        }
      }
    }
  }

  return 0;
}
