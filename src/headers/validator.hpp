#pragma once

#include "toml++/toml.hpp"
#include <string>
#include <vector>

struct ValidationResult {
  enum class Type { Warning, Error };

  struct Entries {
    std::string message;
    Type type;
  };

  std::vector<Entries> log;

  bool has_errors() const;
  bool has_warnings() const;

  void add_error(std::string msg);
  void add_warning(std::string msg);
};

void print_validation(ValidationResult& result);
ValidationResult validator(const toml::table& file);
