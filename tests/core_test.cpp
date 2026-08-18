// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/domain.hpp"

#include <cmath>
#include <iostream>

int main() {
    using namespace sister::praxis;

    const auto coverage = evidence_coverage(8U, 10U);
    if (std::abs(coverage - 0.8) > 1e-12) {
        std::cerr << "coverage mismatch\n";
        return 1;
    }

    const Measurement measurement{"M1", coverage, "ratio", {"E1", "E2"}};
    const Criterion criterion{"C1", 0.8, "minimum evidence coverage"};
    const auto assessment = assess_minimum("A1", "EC1@v1", "S1", measurement, criterion);

    if (assessment.state != EpistemicState::supported) {
        std::cerr << "expected supported assessment\n";
        return 2;
    }
    if (assessment.contract_id != "EC1@v1") {
        std::cerr << "assessment lost contract identity\n";
        return 3;
    }
    if (assessment.evidence_refs.size() != 2U) {
        std::cerr << "assessment lost evidence lineage\n";
        return 4;
    }

    std::cout << "PASS sister_praxis_core_test\n";
    return 0;
}
