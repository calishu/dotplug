#pragma once

#include <stdexcept>

namespace dotplug {

class MissingProfile : public std::runtime_error {
    [[nodiscard]] auto what() const noexcept -> const char* override {
        return "Missing Profile. Please run `dotplug recovery`";
    }
};

class MissingItem : public std::runtime_error {
    [[nodiscard]] auto what() const noexcept -> const char* override {
        return "Missing Item in Profile. Please run `dotplug recovery`";
    }
};

}
