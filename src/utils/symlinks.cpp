#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "context.hpp"
#include "settings.hpp"
#include "utils/config.hpp"

/* @brief Removes every active symlink from the config!
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

  const Config active_config = Config(active_config_name);
  const std::vector<std::string> dependencies = active_config.get_dependencies();

  for (std::string dep_name : dependencies) {
    std::unordered_map<std::string, std::string> dep = active_config.get_dependency(dep_name);

    if(!std::filesystem::exists(dep["destination"])) {
      std::cout << "[ERROR] Couldn't find the destination." << std::endl;
      return 1;
    }

    if (!std::filesystem::is_symlink(dep["destination"]) && ctx->forced == false) {
      std::cout << "[WARNING] " << dep["destination"] << "isn't a symlink! Want to delete it? (y/N)";
      std::string choice;
      std::getline(std::cin, choice);

      if (choice == "n" || choice == "N" || choice == "") continue;
    }

    std::error_code err;
    bool removed = std::filesystem::remove(dep["destination"], err);
    if (!removed || err) {
      std::cout << "Oh no, there was an error :<\n" << err.message() << std::endl;
      return 1;
    }
  }

  std::filesystem::remove(config_path + "current");
  return 0;
}
