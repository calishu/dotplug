#pragma once

#include <cstdlib>
#include <string>

//
/* * config path is where you want to store the configurations regarding to dotplug.
 *
 * * dotfiles path is where the actual configurations you want to manage with
 *   dotplug is stored.
 *
 * * protected path is the part nobody should even think about touching it,
 *   because there the internal files.
 *
 * * direction to the current path is stored) destination path is where the
 *   dotfiles should go (e.g. ~/.config/); deprecated soon.
 * */

// clang-format off
inline const auto home = std::string{getenv("HOME")},
                  state_path = home + STATE_PATH,
                  config_path = home + CONFIG_PATH,
                  dotfiles_path = home + DOTFILES_PATH,
                  destination_path = home + DESTINATION_PATH;
