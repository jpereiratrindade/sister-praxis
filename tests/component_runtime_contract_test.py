#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later

import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
descriptor = json.loads(
    (ROOT / ".sister" / "component.json").read_text(encoding="utf-8")
)
runtime = (ROOT / descriptor["runtime"]["entrypoint"]).read_text(
    encoding="utf-8"
)

assert descriptor["component_id"] == "praxis"
assert descriptor["system_id"] == "sister_praxis"
assert "SISTER_RESOLVED_DEPLOYMENT_FILE" in runtime
assert ".components[] | select(.system_id == $id)" in runtime
assert "SISTER_PRAXIS_HTTP_BIND" in runtime
assert "SISTER_PRAXIS_HTTP_PORT" in runtime

print("[PASS] Praxis component consumes its resolved runtime binding")
