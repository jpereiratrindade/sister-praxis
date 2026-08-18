// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/audit.hpp"
#include "sister/praxis/domain.hpp"

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    using namespace sister::praxis;

    const std::string command = argc > 1 ? argv[1] : "help";
    if (command != "demo") {
        std::cout << "SisTer-Praxis A0 - governed evaluation core\n"
                  << "uso: sister-praxis-cli demo\n";
        return 0;
    }

    const Evidence e1{"E-001", "registro resolvivel", "artifact://E-001", true};
    const Evidence e2{"E-002", "registro resolvivel", "artifact://E-002", true};
    const Measurement m{
        "M-evidence-coverage",
        evidence_coverage(8U, 10U),
        "ratio",
        {e1.id, e2.id}
    };
    const Criterion c{
        "C-min-evidence-coverage",
        0.80,
        "cobertura de evidencia deve atingir o minimo contratado"
    };
    const auto a = assess_minimum("ASS-001", "PRA-EVAL-001@v1", "S-017", m, c);

    std::cout << "measurement=" << m.value << '\n'
              << "assessment=" << to_string(a.state) << '\n';

    audit::append_jsonl(
        std::filesystem::path{"var/audit/events.jsonl"},
        "assessment.recorded",
        a.subject_state_id,
        a.id,
        std::string{to_string(a.state)});

    if (a.state == EpistemicState::supported) {
        const Decision d{
            "DEC-001",
            {a.id},
            "proceed_with_successor",
            "assessment supported under contracted evidence criterion",
            "project_coordinator",
            "S-018"
        };
        audit::append_jsonl(
            std::filesystem::path{"var/audit/events.jsonl"},
            "decision.authorized",
            d.successor_state_id,
            d.id,
            d.rationale);
        std::cout << "decision=" << d.selected_alternative << '\n'
                  << "successor=" << d.successor_state_id << '\n';
    }

    return 0;
}
