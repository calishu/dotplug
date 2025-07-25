#pragma once

#include <cstdlib>
#include <string>

// clang-format off
inline const auto home = std::string{getenv("HOME")},
				  config_path = home + CONFIG_PATH,
                  dotfiles_path = home + DOTFILES_PATH,
                  destination_path = home + DESTINATION_PATH;
