#include "toml++/toml.hpp"
#include "config.hpp"
#include <iostream>
#include <format>
#include <string>

struct ValidationResult {
  enum class Type { Warning, Error };

  struct Entries {
    std::string message;
    Type type;
  };
  std::vector<Entries> log;

  void add_error(std::string msg) {
    log.push_back({ msg, Type::Error });
  }
  
  void add_warning(std::string msg) {
    log.push_back({ msg, Type::Warning });
  }
};

void print_validation(ValidationResult& result) {
  if (result.log.empty()) std::cout << "Everything is fine with the configuration." << std::endl;
  for (const auto& e : result.log) {
    std::cout << (e.type == ValidationResult::Type::Error ? "[ERROR] " : "[WARNING] ") << e.message << "\n";
  }
  return;
}

ValidationResult validator(const toml::table& file) {
  ValidationResult output;

  auto dependencies = file["dotplug"]["dependencies"].as_array();
  if (!dependencies || dependencies->empty()) {
    output.add_error("You need at least one dependency");
    return output;
  }

  for (size_t i = 0; i < dependencies->size(); ++i) {
    auto& dep_node = (*dependencies)[i];
    auto dep_name_opt = dep_node.value<std::string>();
    if (!dep_name_opt) {
      output.add_error("Dependency is not a string");
      continue;
    }
    const std::string& dep_name = *dep_name_opt;

    if (!file["dotplug"][dep_name] || dependencies->empty()) {
      output.add_error(format("Dependency section '{}' is missing", dep_name));
    } 

    const auto& dep_entry = file["dotplug"][dep_name]["destination"];
    if (!dep_entry)
      output.add_error("You need to enter a destination for your dependency");

    auto dep_source_result = dep_source(file, dep_name);
    if (dep_source_result.second == 1) {
      output.add_error(format("Couldn't find the source of '{}'", dep_name));
    }
  }

  return output; 
}
