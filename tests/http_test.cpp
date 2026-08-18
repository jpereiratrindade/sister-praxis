// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/http.hpp"
#include "sister/praxis/runtime.hpp"

#include <cassert>
#include <string>

int main() {
    using namespace sister::praxis;

    runtime::ServiceRegistry registry;
    registry.register_service(
        runtime::ServiceDescriptor{
            .id = "test.echo",
            .owner = "test-domain",
            .kind = runtime::TransformationKind::analysis,
            .contract_ref = "test.echo@v1",
            .implementation_ref = "builtin:test-echo@1",
            .version = "1.0.0",
            .description = "test service",
        },
        [](const runtime::TransformationRequest& request) {
            return runtime::TransformationOutput{
                .successor_state_id = request.predecessor_state_id + ".next",
                .output_refs = {},
                .provenance_ref = "prov://test/" + request.run_id,
            };
        });

    const http::Application app{registry};

    const auto health = app.handle("GET", "/health");
    assert(health.status == 200);
    assert(health.body.find("\"system\":\"sister_praxis\"") != std::string::npos);
    assert(health.body.find("\"http\":\"ready\"") != std::string::npos);

    const auto services = app.handle("GET", "/services");
    assert(services.status == 200);
    assert(services.body.find("\"id\":\"test.echo\"") != std::string::npos);
    assert(services.body.find("\"owner\":\"test-domain\"") != std::string::npos);

    assert(app.handle("GET", "/missing").status == 404);
    assert(app.handle("POST", "/services").status == 405);

    const auto wire = http::serialize_response(health);
    assert(wire.find("HTTP/1.1 200 OK\r\n") == 0U);
    assert(wire.find("Cache-Control: no-store\r\n") != std::string::npos);

    return 0;
}
