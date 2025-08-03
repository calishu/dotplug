# Project Overview
The name of the project is "dotplug", it's a dotfile manager built for UNIX-based systems (e.g. linux or macOS).
It's built on C++ with meson as primary build tool (while CMake is also supported) and conan for the dependencies.
It utilizes symlinks for managing the dotfile configurations.

## Folder Structure
- `/src`: Contains the source code of the CLI tool
- `/include`: Contains the header files of `/src`
- `/lang`: Contains the translations in `.json` files.

## Libraries
- `nlohmann_json`: Is used for processing persistent data (e.g. internal configurations/states, translations)
- `tomlplusplus`: Is used for user-made configurations for e.g. configurations about a dotfile setup, stating source and destination
- `CLI11`: Is used for handling the CLI (Commandline-Interface)

## Code Styles
- Indentations: Use 4 spaces, not tabs
- Pointer/Reference Alignment: Align pointers must face the variable name (e.g. `std::string &str`)
- Line Lenght: Aim for a maximum line length of 120 characters
- Braces: Follow the `one true brace style` (also called `1tbs`)
- Includes: Always use `#include <[...]>` for standard/external libraries and `#include "[...]"` for project-local headers

## Pull Request and Commit Rules
- PR scope: Each PR should address a single feature or bug, except for refactors of the whole project.
- PR description: The description must be clearly explained, meaning the "what" and "why" of changes
- Commit Messages: Follow the [Conventional Commits](https://www.conventionalcommits.org/) specifications (e.g. `feat(CLI): <message>`, `fix(logging): <message>`)
- Language: All user-facing strings must be added to the JSON translation files in `/lang` and focus on `en_US`. Do not hardcode strings.
---
For more Rules regarding Pull Requests, Commits and Code Style, check [CONTRIBUTING.md](https://github.com/calishu/dotplug/blob/main/.github/CONTRIBUTING.md) and [.clang-format](https://github.com/calishu/blob/main/.clang-format) for more detailed information.
