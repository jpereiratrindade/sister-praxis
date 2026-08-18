#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

python3 - <<'PY'
import json
from pathlib import Path
for path in sorted(Path('contracts').glob('*.json')):
    json.loads(path.read_text())
    print(f'[PASS] JSON syntax {path}')
PY

GEN=()
if command -v ninja >/dev/null 2>&1; then GEN=(-G Ninja); fi
cmake -S . -B build "${GEN[@]}" -DCMAKE_BUILD_TYPE=Debug -DSISTER_PRAXIS_WARNINGS_AS_ERRORS=ON
cmake --build build
ctest --test-dir build --output-on-failure
./build/sister-praxis-cli demo
printf '\n[PASS] SisTer-Praxis A0-R1 core/runtime verification\n'
