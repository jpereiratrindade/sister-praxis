// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sister::praxis::runtime {

enum class TransformationKind {
    analysis,
    derivation,
    aggregation,
    comparison,
    evaluation,
    projection,
    conversion
};

struct TemporalContext {
    std::string observed_at;
    std::string valid_at;
};

struct ServiceDescriptor {
    std::string id;
    std::string owner;
    TransformationKind kind{TransformationKind::analysis};
    std::string contract_ref;
    std::string implementation_ref;
    std::string version;
    std::string description;
};

struct TransformationRequest {
    std::string run_id;
    std::string service_id;
    std::string predecessor_state_id;
    std::vector<std::string> input_refs;
    std::vector<std::pair<std::string, std::string>> parameters;
    TemporalContext temporal_context;
};

struct TransformationOutput {
    std::string successor_state_id;
    std::vector<std::string> output_refs;
    std::string provenance_ref;
};

struct TransformationReceipt {
    std::string run_id;
    std::string service_id;
    std::string owner;
    TransformationKind kind{TransformationKind::analysis};
    std::string contract_ref;
    std::string implementation_ref;
    std::string version;
    std::string predecessor_state_id;
    std::string successor_state_id;
    std::vector<std::string> input_refs;
    std::vector<std::string> output_refs;
    std::vector<std::pair<std::string, std::string>> parameters;
    TemporalContext temporal_context;
    std::string provenance_ref;
};

using ServiceExecutor = std::function<TransformationOutput(const TransformationRequest&)>;

class ServiceRegistry {
public:
    void register_service(ServiceDescriptor descriptor, ServiceExecutor executor);

    [[nodiscard]] bool contains(std::string_view service_id) const;
    [[nodiscard]] std::vector<ServiceDescriptor> services() const;
    [[nodiscard]] TransformationReceipt execute(const TransformationRequest& request) const;

private:
    struct Entry {
        ServiceDescriptor descriptor;
        ServiceExecutor executor;
    };

    std::unordered_map<std::string, Entry> entries_;
};

[[nodiscard]] std::string_view to_string(TransformationKind kind) noexcept;

} // namespace sister::praxis::runtime
