// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/runtime.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    using namespace sister::praxis::runtime;

    ServiceRegistry registry;
    registry.register_service(
        ServiceDescriptor{
            .id = "example.state.derive",
            .owner = "example-domain",
            .kind = TransformationKind::derivation,
            .contract_ref = "example.state.derive@v1",
            .implementation_ref = "git:example-domain@abc123",
            .version = "1.0.0",
            .description = "test service whose semantics remain owned outside Praxis Core",
        },
        [](const TransformationRequest& request) {
            if (request.input_refs.empty()) {
                throw std::invalid_argument("test service requires input");
            }
            return TransformationOutput{
                .successor_state_id = "STATE-002",
                .output_refs = {"artifact://derived/001"},
                .provenance_ref = "prov://run/001",
            };
        });

    if (!registry.contains("example.state.derive")) {
        std::cerr << "registered service missing\n";
        return 1;
    }

    const auto listed = registry.services();
    if (listed.size() != 1U || listed.front().owner != "example-domain") {
        std::cerr << "service ownership was not preserved\n";
        return 2;
    }

    const auto receipt = registry.execute(TransformationRequest{
        .run_id = "RUN-001",
        .service_id = "example.state.derive",
        .predecessor_state_id = "STATE-001",
        .input_refs = {"artifact://input/001"},
        .parameters = {{"method", "example"}},
        .temporal_context = {.observed_at = "2026-08-18T14:05:00-03:00", .valid_at = "2026-08-18"},
    });

    if (receipt.predecessor_state_id != "STATE-001" || receipt.successor_state_id != "STATE-002") {
        std::cerr << "state transition identity lost\n";
        return 3;
    }
    if (receipt.contract_ref != "example.state.derive@v1") {
        std::cerr << "contract identity lost\n";
        return 4;
    }
    if (receipt.implementation_ref != "git:example-domain@abc123") {
        std::cerr << "implementation identity lost\n";
        return 5;
    }
    if (receipt.provenance_ref != "prov://run/001") {
        std::cerr << "provenance identity lost\n";
        return 6;
    }
    if (to_string(receipt.kind) != "derivation") {
        std::cerr << "transformation kind mismatch\n";
        return 7;
    }

    bool duplicate_rejected = false;
    try {
        registry.register_service(
            ServiceDescriptor{
                .id = "example.state.derive",
                .owner = "another-owner",
                .kind = TransformationKind::analysis,
                .contract_ref = "duplicate@v1",
                .implementation_ref = "git:duplicate",
                .version = "1.0.0",
                .description = "duplicate",
            },
            [](const TransformationRequest&) { return TransformationOutput{}; });
    } catch (const std::invalid_argument&) {
        duplicate_rejected = true;
    }
    if (!duplicate_rejected) {
        std::cerr << "duplicate service registration was accepted\n";
        return 8;
    }

    std::cout << "PASS sister_praxis_runtime_test\n";
    return 0;
}
