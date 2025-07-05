#pragma once

#include <cstdlib>
#include <string>

inline std::string dotfiles_path = std::string(std::getenv("HOME")) + DOTFILES_PATH;
