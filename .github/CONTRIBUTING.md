# Contribution Guide

## Table of Content
- [Getting Started](#getting-started)
- [Code Style](#code-style)
- [C++ specific style](#c++-specific-style)
- [Translation Guide](#translation-guide)
- [Pull Requests](#pull-requests)

## Getting Started
1. Fork this repo.
2. Run `git clone [...]` with the `.git` URL of the fork.
3. Install [`git`](https://git-scm.com/), [`meson`](https://mesonbuild.com/) and [`conan`](https://conan.io/) on your system.
4. Setup `conan` for your system with `conan profile new default --detect`.
5. Go inside the project folder and install the dependencies with `conan install . --output-folder=build --build=missing`
6. Now setup `meson` with `meson setup build`
Now you're basically done with the process and you can start with the development.
For compiling the project run `meson compile -C build` in the terminal, after that you can dotplug using `./build/dotplug`.
For changes directly involving meson, you should consider using the `--reconfigure` flag.

## Code Style
- Indentation: Use 4 spaces, no tabs
- Alignment for Pointers/References: Align them facing the variable name (e.g. `std::string &str`).
- Line-Length: Lines must be below 120 characters, unless it isn't possible.
- Braces: Follow the `one true brace style` (also called `1tbs`), refer to this [Wikipedia artice](https://en.wikipedia.org/wiki/Indentation_style#One_True_Brace).
+ Includes: 
  1. Use `#include <[...]>` for standard/external libraries and `#include "[...]"` for local headers.
  2. The standard/external included must be the first one, followed by the local headers.
  3. The order of the includes are alphabetical.
+ Language:
  1. Do **NOT** hardcode user-facing strings, they must be added to the JSON translation.
  2. In development of this project the focus lays on `/lang/en_US.json`.

## C++ specific style
- Don't access public members of objects, rather create a getter function in that class.
- Use `auto`, `const` and references (`&`) as much as possible.
- Use `auto function([...]) -> return_type` declaration syntax, inspired by modern programming languages.
- Use brace-initializers for object constructor calls. It's subjective, but this helps us speparate between normal functions calls from constructors.
- Use short and clear namespace aliases when it's too long. (e.g. `namespace fs = std::filesystem;`)
- Always use `std::vector::emplace_back` over `push_back`.
*Read [this Pull Request](https://github.com/calishu/dotplug/pull/5) for a more clear description of the reason.*

## Translation Guide
1. Follow the [Getting Started](#getting-started) Guide.
2. Use `/lang/en_US.json` as reference, or if it's a new translation just copy it over and replace it.
3. It's always a good to compile the program and do a few checks.

## Pull Requests
- PR scope: Each PR should focus a single feature or bug, exceptions will happen if it's a total refactor.
+ PR description: The feature/fix proposed in the PR should be clearly explained in the description.
  1. What is the purpose of this PR?
  2. Explain it easy terms what had changed.
  3. Explain how it was done technically.
  4. Can you provide us possible edge-cases or notes we should know?
+ Commits: Follow the [Conventional Commits](https://conventionalcommits.org/) specifications.
  1. Use tags like `feat`, `fix` or `build`.
  2. Your commit message must be clear.
  3. If the changes are bigger, consider adding a detailed commit description.
