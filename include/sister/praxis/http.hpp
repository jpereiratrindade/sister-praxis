// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "sister/praxis/runtime.hpp"

#include <string>
#include <string_view>

namespace sister::praxis::http {

struct Response {
    int status{200};
    std::string content_type{"application/json; charset=utf-8"};
    std::string body;
};

class Application {
public:
    explicit Application(const runtime::ServiceRegistry& registry) noexcept;

    [[nodiscard]] Response handle(std::string_view method, std::string_view path) const;

private:
    const runtime::ServiceRegistry& registry_;
};

[[nodiscard]] std::string reason_phrase(int status);
[[nodiscard]] std::string serialize_response(const Response& response);

} // namespace sister::praxis::http
