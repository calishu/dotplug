#pragma once

#include "include/core/profiles.hpp"
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Core {

class Root {
    json m_root_json;
    Core::Profile m_active_profile;
    std::vector<Core::Profile> m_profiles;

public:
    Root();
    ~Root() = default;

    auto addProfile(Core::Profile& profile) -> int;
    auto removeProfile(Core::Profile& profile) -> int;
    auto switchActiveProfile(Core::Profile& profile) -> int;

    auto getActiveProfile() -> Core::Profile& {
        return m_active_profile;
    }

    auto getAllProfiles() -> std::vector<Core::Profile>& {
        return m_profiles;
    }
};

}
