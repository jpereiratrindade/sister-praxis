// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace sister::praxis {

enum class EpistemicState {
    observed,
    supported,
    verified,
    inconclusive,
    rejected,
    authorized,
    adopted
};

struct Evidence {
    std::string id;
    std::string description;
    std::string provenance_ref;
    bool resolvable{false};
};

struct Measurement {
    std::string id;
    double value{0.0};
    std::string unit;
    std::vector<std::string> evidence_refs;
};

struct Criterion {
    std::string id;
    double minimum{0.0};
    std::string rationale;
};

struct Assessment {
    std::string id;
    std::string contract_id;
    std::string subject_state_id;
    EpistemicState state{EpistemicState::inconclusive};
    double value{0.0};
    std::string rationale;
    std::vector<std::string> evidence_refs;
};

struct Decision {
    std::string id;
    std::vector<std::string> assessment_refs;
    std::string selected_alternative;
    std::string rationale;
    std::string authority;
    std::string successor_state_id;
};

[[nodiscard]] double evidence_coverage(std::size_t resolvable, std::size_t total);

[[nodiscard]] Assessment assess_minimum(
    std::string id,
    std::string contract_id,
    std::string subject_state_id,
    const Measurement& measurement,
    const Criterion& criterion);

[[nodiscard]] std::string_view to_string(EpistemicState state) noexcept;

} // namespace sister::praxis
