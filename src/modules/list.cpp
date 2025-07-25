#include <cstdlib>
#include <filesystem>
#include <string>

#include "context.hpp"
#include "modules/list.hpp"
#include "settings.hpp"
#include "utils/config.hpp"

namespace fs = std::filesystem;

void list() {
    if (!ctx->name.empty()) {
        Config{ctx->name}.print();
        return;
    }

    size_t i = 1;
    for (const auto &entry : fs::directory_iterator{dotfiles_path})
        Config{entry.path().filename().string()}.print(std::to_string(i++));
}
