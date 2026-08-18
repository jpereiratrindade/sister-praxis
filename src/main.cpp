// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/audit.hpp"
#include "sister/praxis/domain.hpp"
#include "sister/praxis/runtime.hpp"

#include <filesystem>
#include <iostream>
#include <string>

namespace {

void run_core_demo() {
    using namespace sister::praxis;

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

    std::cout << "core.measurement=" << m.value << '\n'
              << "core.assessment=" << to_string(a.state) << '\n';

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
        std::cout << "core.decision=" << d.selected_alternative << '\n'
                  << "core.successor=" << d.successor_state_id << '\n';
    }
}

void run_runtime_demo() {
    using namespace sister::praxis::runtime;

    ServiceRegistry registry;
    registry.register_service(
        ServiceDescriptor{
            .id = "demo.state.derive",
            .owner = "demo-domain",
            .kind = TransformationKind::derivation,
            .contract_ref = "demo.state.derive@v1",
            .implementation_ref = "builtin:demo-state-derive@1",
            .version = "1.0.0",
            .description = "demonstrates domain-owned semantics executed by the shared Praxis runtime",
        },
        [](const TransformationRequest&) {
            return TransformationOutput{
                .successor_state_id = "S-019",
                .output_refs = {"artifact://demo/derived-state"},
                .provenance_ref = "prov://demo/RUN-001",
            };
        });

    const auto receipt = registry.execute(TransformationRequest{
        .run_id = "RUN-001",
        .service_id = "demo.state.derive",
        .predecessor_state_id = "S-018",
        .input_refs = {"artifact://demo/input-state"},
        .parameters = {{"mode", "demonstration"}},
        .temporal_context = {.observed_at = "2026-08-18T14:05:00-03:00", .valid_at = "2026-08-18"},
    });

    std::cout << "runtime.service=" << receipt.service_id << '\n'
              << "runtime.owner=" << receipt.owner << '\n'
              << "runtime.kind=" << to_string(receipt.kind) << '\n'
              << "runtime.predecessor=" << receipt.predecessor_state_id << '\n'
              << "runtime.successor=" << receipt.successor_state_id << '\n'
              << "runtime.provenance=" << receipt.provenance_ref << '\n';

    sister::praxis::audit::append_jsonl(
        std::filesystem::path{"var/audit/events.jsonl"},
        "transformation.executed",
        receipt.successor_state_id,
        receipt.run_id,
        receipt.service_id);
}

} // namespace

int main(int argc, char** argv) {
    const std::string command = argc > 1 ? argv[1] : "help";
    if (command != "demo" && command != "demo-core" && command != "demo-runtime") {
        std::cout << "SisTer-Praxis v0.2 - core + governed transformation runtime\n"
                  << "uso: sister-praxis-cli demo|demo-core|demo-runtime\n";
        return 0;
    }

    if (command == "demo" || command == "demo-core") {
        run_core_demo();
    }
    if (command == "demo" || command == "demo-runtime") {
        run_runtime_demo();
    }

    return 0;
}
