#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cat "$ROOT/.hoa/upstream-praxis.yaml"
if [[ -f "$ROOT/var/audit/events.jsonl" ]]; then
  echo
  tail -20 "$ROOT/var/audit/events.jsonl"
fi
