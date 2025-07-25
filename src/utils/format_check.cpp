#include <string>

#include "context.hpp"

/* @brief Checks if the value contains `://`.
 *
 * The value is defined by the `name` string in the global pointer context.
 * (See reference at `src/context.cpp` and `src/headers/context.hpp`)
 *
 * @return true if the string contains `://`, false if not.
 */
bool is_valid_url() {
    const auto pos = ctx->name.find("://");
    return (pos != std::string::npos) && (pos < 10);
}
