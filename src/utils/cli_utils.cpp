#include "cli_utils.hpp"

CLI::Option* add_text_option(CLI::Option_group* group, std::string& value, const std::string& name, const std::string& description) {
  return group->add_option(name, value, description)
    ->check([](const std::string& val) -> std::string {
      return value.empty() ? name + " cannot be empty" : "";
  });
}
