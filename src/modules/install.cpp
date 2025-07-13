#include "install.hpp"

#include "git2.h"
#include "config.hpp"
#include "context.hpp"
#include "settings.hpp"
#include "validator.hpp"
#include "format_check.hpp"
#include <string_view>
#include <filesystem>
#include <iostream>
#include <cstdio>
#include <string>

int install() {
  if (!is_valid_url()) {
    std::cout << "The provided value is not a URL!" << std::endl;
    return 1;
  }

  size_t last_slash = ctx->name.find_last_of("/");
  if (last_slash == std::string::npos) return 1;

  std::string_view name_view = ctx->name.substr(last_slash + 1);

  if (name_view.ends_with(".git"))
    name_view.remove_suffix(4);

  std::string name = std::string(name_view);

  git_libgit2_init();
  if (git_error_last()->message != "no error") {
    std::cout << "Something failed during the git initialization.\n" << git_error_last()->message << std::endl;
    return 1;
  }
  
  std::string clone_path = dotfiles_path + name;
  git_repository* repo_ptr = nullptr;
  const int clone_return = git_clone(&repo_ptr, ctx->name.c_str(), clone_path.c_str(), NULL);

  if (clone_return != 0) {
    std::cout << "Something wen't wrong during the cloning process.\n" << git_error_last()->message << std::endl;
    return 1;
  }

  git_libgit2_shutdown();

  std::cout << "Downloaded the config! Wanna validate it? (Y/n) ";

  std::string validate_choice;
  std::getline(std::cin, validate_choice);
  if (validate_choice.empty() || validate_choice == "n" || validate_choice == "N") return 0;
  
  std::pair<toml::table, int> config = parse_config(name);
  ValidationResult validation_result = validator(config.first);

  if (validation_result.has_errors())
    print_validation(validation_result);

  return 0;
}
