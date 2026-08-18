#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
GEN=()
if command -v ninja >/dev/null 2>&1; then GEN=(-G Ninja); fi
cmake -S . -B build "${GEN[@]}" -DCMAKE_BUILD_TYPE=Debug -DSISTER_PRAXIS_WARNINGS_AS_ERRORS=ON
cmake --build build
ctest --test-dir build --output-on-failure
./build/sister-praxis-cli demo
printf '\n[PASS] SisTer-Praxis A0 verification\n'
