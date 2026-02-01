#pragma once

#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Core {

struct Item {
    std::string name;
    std::string location;
    std::string id; // sha256 hash of the name
};

class Profile {
    json m_profile_json;
    std::vector<Core::Item> m_items;

public:
    ~Profile() = default;

    // `loadProfile` and `newProfile` only works if m_items and m_profile_json wasn't used yet
    auto loadProfile(std::string_view id) -> int;
    auto newProfile(std::string_view name) -> int;

    auto addItem(Core::Item& item) -> int;
    auto removeItem(Core::Item& item) -> int;

    auto getAllItems() -> std::vector<Core::Item>& {
        return m_items;
    };
};

}
