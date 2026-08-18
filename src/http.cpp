// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/http.hpp"

#include <sstream>

namespace sister::praxis::http {
namespace {

std::string json_escape(const std::string_view value) {
    std::string out;
    out.reserve(value.size());
    for (const char ch : value) {
        switch (ch) {
            case '\\': out += "\\\\"; break;
            case '"': out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += ch; break;
        }
    }
    return out;
}

Response health_response() {
    return Response{
        .status = 200,
        .content_type = "application/json; charset=utf-8",
        .body = R"({"system":"sister_praxis","status":"ready","core":"ready","runtime":"ready","http":"ready","version":"0.2.1"})",
    };
}

Response services_response(const runtime::ServiceRegistry& registry) {
    std::ostringstream out;
    out << "{\"services\":[";
    bool first = true;
    for (const auto& service : registry.services()) {
        if (!first) {
            out << ',';
        }
        first = false;
        out << "{\"id\":\"" << json_escape(service.id)
            << "\",\"owner\":\"" << json_escape(service.owner)
            << "\",\"kind\":\"" << runtime::to_string(service.kind)
            << "\",\"contract_ref\":\"" << json_escape(service.contract_ref)
            << "\",\"implementation_ref\":\"" << json_escape(service.implementation_ref)
            << "\",\"version\":\"" << json_escape(service.version)
            << "\",\"description\":\"" << json_escape(service.description)
            << "\"}";
    }
    out << "]}";
    return Response{.status = 200, .content_type = "application/json; charset=utf-8", .body = out.str()};
}

} // namespace

Application::Application(const runtime::ServiceRegistry& registry) noexcept : registry_(registry) {}

Response Application::handle(const std::string_view method, const std::string_view path) const {
    if (method != "GET") {
        return Response{.status = 405, .body = R"({"error":"method_not_allowed"})"};
    }
    if (path == "/health") {
        return health_response();
    }
    if (path == "/services") {
        return services_response(registry_);
    }
    return Response{.status = 404, .body = R"({"error":"not_found"})"};
}

std::string reason_phrase(const int status) {
    switch (status) {
        case 200: return "OK";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        default: return "Error";
    }
}

std::string serialize_response(const Response& response) {
    std::ostringstream out;
    out << "HTTP/1.1 " << response.status << ' ' << reason_phrase(response.status) << "\r\n"
        << "Content-Type: " << response.content_type << "\r\n"
        << "Content-Length: " << response.body.size() << "\r\n"
        << "Connection: close\r\n"
        << "Cache-Control: no-store\r\n"
        << "\r\n"
        << response.body;
    return out.str();
}

} // namespace sister::praxis::http
