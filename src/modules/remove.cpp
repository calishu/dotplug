#include <filesystem>
#include <string>

#include "context.hpp"
#include "settings.hpp"
#include "utils/logging.hpp"

namespace fs = std::filesystem;

int remove_config() {
    const auto path = dotfiles_path + ctx->name;

    if (!fs::exists(path)) {
        ctx->logging->log(LoggingLevel::ERROR, ctx->locale.json["remove"]["errors"]["doesnt_exist"]);
        return 1;
    }

    if (!ctx->forced) {
        if (!std::get<bool>(
                ctx->logging->prompt(PromptMode::BOOL, ctx->locale.json["remove"]["prompts"]["confirmation"], "1")))
            return 0;
    }

    fs::remove_all(path);
    return 0;
}
