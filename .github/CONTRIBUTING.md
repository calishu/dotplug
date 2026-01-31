# Contribution Guide
> [!NOTE]
> When contributing, please refer to the [Code of Conduct](https://github.com/calishu/dotplug/blob/main/.github/CODE_OF_CONDUCT.md)

## Table of Content
- [Code Style](#code-style)
- [C++ specific style](#c-specific-style)
- [Translation Guide](#translation-guide)
- [Pull Requests](#pull-requests)

## Code Style
- Indentation: Use 4 spaces, no tabs
- Alignment for Pointers/References: Align them facing the variable name (e.g. `std::string &str`).
- Line-Length: Lines must be below 120 characters, unless it isn't possible.
- Braces: Follow the `one true brace style` (also called `1tbs`), refer to this [Wikipedia article](https://en.wikipedia.org/wiki/Indentation_style#One_True_Brace).
- Includes: 
  1. Use `#include <[...]>` for standard/external libraries and `#include "[...]"` for local headers.
  2. The standard/external included must be the first one, followed by the local headers.
  3. The order of the includes are alphabetical.

*Please format your code using [clang-format](https://github.com/calishu/dotplug/blob/main/.clang-format), or just run the [`format.sh`](https://github.com/calishu/dotplug/blob/main/format.sh) file.*

## C++ specific style
- Don't access public members of objects, rather create a getter function in that class.
- Use `auto`, `const` and references (`&`) as much as possible.
- Use `auto function([...]) -> return_type` declaration syntax, inspired by modern programming languages.
- Use brace-initializers for object constructor calls. It's subjective, but this helps us speparate between normal functions calls from constructors.
- Use short and clear namespace aliases when it's too long. (e.g. `namespace fs = std::filesystem;`)
- Always use `std::vector::emplace_back` over `push_back`.

*Read [this Pull Request](https://github.com/calishu/dotplug/pull/5) for a more clear description of the reason.*

## Pull Requests
- PR scope: Each PR should focus a single feature or bug, exceptions will happen if it's a total refactor.
- PR description: The feature/fix proposed in the PR should be clearly explained in the description.
  1. What is the purpose of this PR?
  2. Explain it easy terms what had changed.
  3. Explain how it was done technically.
  4. Can you provide us possible edge-cases or notes we should know?
- Commits: Follow the [Conventional Commits](https://conventionalcommits.org/) specifications.
  1. Use tags like `feat`, `fix` or `build`.
  2. Your commit message must be clear.
  3. If the changes are bigger, consider adding a detailed commit description.
