#pragma once

#include <cstdlib>
#include <string>

inline std::string config_path      = std::string(std::getenv("HOME")) + CONFIG_PATH;
inline std::string dotfiles_path    = std::string(std::getenv("HOME")) + DOTFILES_PATH;
inline std::string destination_path = std::string(std::getenv("HOME")) + DESTINATION_PATH;
