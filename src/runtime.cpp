// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/runtime.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace sister::praxis::runtime {
namespace {

void require_non_empty(const std::string& value, const char* field) {
    if (value.empty()) {
        throw std::invalid_argument(std::string{field} + " must not be empty");
    }
}

} // namespace

void ServiceRegistry::register_service(ServiceDescriptor descriptor, ServiceExecutor executor) {
    require_non_empty(descriptor.id, "service id");
    require_non_empty(descriptor.owner, "service owner");
    require_non_empty(descriptor.contract_ref, "contract reference");
    require_non_empty(descriptor.implementation_ref, "implementation reference");
    require_non_empty(descriptor.version, "service version");
    if (!executor) {
        throw std::invalid_argument("service executor must be callable");
    }

    const auto service_id = descriptor.id;
    const auto [it, inserted] = entries_.emplace(
        service_id,
        Entry{.descriptor = std::move(descriptor), .executor = std::move(executor)});
    (void)it;
    if (!inserted) {
        throw std::invalid_argument("service already registered: " + service_id);
    }
}

bool ServiceRegistry::contains(const std::string_view service_id) const {
    return entries_.contains(std::string{service_id});
}

std::vector<ServiceDescriptor> ServiceRegistry::services() const {
    std::vector<ServiceDescriptor> out;
    out.reserve(entries_.size());
    for (const auto& [id, entry] : entries_) {
        (void)id;
        out.push_back(entry.descriptor);
    }
    std::ranges::sort(out, {}, &ServiceDescriptor::id);
    return out;
}

TransformationReceipt ServiceRegistry::execute(const TransformationRequest& request) const {
    require_non_empty(request.run_id, "run id");
    require_non_empty(request.service_id, "service id");
    require_non_empty(request.predecessor_state_id, "predecessor state id");

    const auto found = entries_.find(request.service_id);
    if (found == entries_.end()) {
        throw std::invalid_argument("unknown service: " + request.service_id);
    }

    const auto output = found->second.executor(request);
    require_non_empty(output.successor_state_id, "successor state id");
    require_non_empty(output.provenance_ref, "provenance reference");

    const auto& descriptor = found->second.descriptor;
    return TransformationReceipt{
        .run_id = request.run_id,
        .service_id = descriptor.id,
        .owner = descriptor.owner,
        .kind = descriptor.kind,
        .contract_ref = descriptor.contract_ref,
        .implementation_ref = descriptor.implementation_ref,
        .version = descriptor.version,
        .predecessor_state_id = request.predecessor_state_id,
        .successor_state_id = output.successor_state_id,
        .input_refs = request.input_refs,
        .output_refs = output.output_refs,
        .parameters = request.parameters,
        .temporal_context = request.temporal_context,
        .provenance_ref = output.provenance_ref,
    };
}

std::string_view to_string(const TransformationKind kind) noexcept {
    switch (kind) {
        case TransformationKind::analysis: return "analysis";
        case TransformationKind::derivation: return "derivation";
        case TransformationKind::aggregation: return "aggregation";
        case TransformationKind::comparison: return "comparison";
        case TransformationKind::evaluation: return "evaluation";
        case TransformationKind::projection: return "projection";
        case TransformationKind::conversion: return "conversion";
    }
    return "unknown";
}

} // namespace sister::praxis::runtime
