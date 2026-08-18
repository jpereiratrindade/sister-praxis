// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/audit.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

namespace sister::praxis::audit {
namespace {

std::string escape_json(std::string_view text) {
    std::string out;
    out.reserve(text.size() + 8U);
    for (const char c : text) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '"': out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

std::string now_utc() {
    const auto now = std::chrono::system_clock::now();
    const auto tt = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &tt);
#else
    gmtime_r(&tt, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

} // namespace

void append_jsonl(
    const std::filesystem::path& path,
    const std::string_view event_type,
    const std::string_view subject_id,
    const std::string_view reference_id,
    const std::string_view message) {

    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }
    std::ofstream out(path, std::ios::app);
    if (!out) {
        throw std::runtime_error("cannot open audit trail: " + path.string());
    }
    out << "{\"at\":\"" << now_utc()
        << "\",\"event\":\"" << escape_json(event_type)
        << "\",\"subject\":\"" << escape_json(subject_id)
        << "\",\"ref\":\"" << escape_json(reference_id)
        << "\",\"message\":\"" << escape_json(message)
        << "\"}\n";
}

} // namespace sister::praxis::audit
