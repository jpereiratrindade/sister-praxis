// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <filesystem>
#include <string_view>

namespace sister::praxis::audit {

void append_jsonl(
    const std::filesystem::path& path,
    std::string_view event_type,
    std::string_view subject_id,
    std::string_view reference_id,
    std::string_view message);

} // namespace sister::praxis::audit
