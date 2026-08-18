// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/domain.hpp"

#include <stdexcept>
#include <utility>

namespace sister::praxis {

double evidence_coverage(const std::size_t resolvable, const std::size_t total) {
    if (total == 0U) {
        return 0.0;
    }
    if (resolvable > total) {
        throw std::invalid_argument("resolvable evidence cannot exceed total evidence");
    }
    return static_cast<double>(resolvable) / static_cast<double>(total);
}

Assessment assess_minimum(
    std::string id,
    std::string contract_id,
    std::string subject_state_id,
    const Measurement& measurement,
    const Criterion& criterion) {

    const auto status = measurement.value >= criterion.minimum
        ? EpistemicState::supported
        : EpistemicState::inconclusive;

    Assessment out{
        .id = std::move(id),
        .contract_id = std::move(contract_id),
        .subject_state_id = std::move(subject_state_id),
        .state = status,
        .value = measurement.value,
        .rationale = criterion.rationale,
        .evidence_refs = measurement.evidence_refs,
    };
    return out;
}

std::string_view to_string(const EpistemicState state) noexcept {
    switch (state) {
        case EpistemicState::observed: return "observed";
        case EpistemicState::supported: return "supported";
        case EpistemicState::verified: return "verified";
        case EpistemicState::inconclusive: return "inconclusive";
        case EpistemicState::rejected: return "rejected";
        case EpistemicState::authorized: return "authorized";
        case EpistemicState::adopted: return "adopted";
    }
    return "unknown";
}

} // namespace sister::praxis
