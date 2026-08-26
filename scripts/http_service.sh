#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
PID_FILE="$ROOT/var/run/sister-praxis-http.pid"
LOG_FILE="$ROOT/var/log/sister-praxis-http.log"
BIN="$ROOT/build/sister-praxis-http"
BIND="${SISTER_PRAXIS_HTTP_BIND:-127.0.0.1}"
PORT="${SISTER_PRAXIS_HTTP_PORT:-8093}"
WEB_INDEX="${SISTER_PRAXIS_WEB_INDEX:-$ROOT/web/index.html}"

mkdir -p "$ROOT/var/run" "$ROOT/var/log"

is_running() {
  [[ -f "$PID_FILE" ]] || return 1
  local pid
  pid="$(cat "$PID_FILE")"
  kill -0 "$pid" 2>/dev/null
}

case "${1:-status}" in
  start)
    if is_running; then
      echo "[PASS] sister-praxis-http already running pid=$(cat "$PID_FILE")"
      exit 0
    fi
    [[ -x "$BIN" ]] || { echo "[FAIL] missing executable: $BIN"; exit 2; }
    if [[ "$BIND" != "127.0.0.1" ]]; then
      echo "[FAIL] A0-R2 requires loopback bind; got $BIND"
      exit 3
    fi
    nohup "$BIN" --bind "$BIND" --port "$PORT" --web-index "$WEB_INDEX" >>"$LOG_FILE" 2>&1 &
    echo $! > "$PID_FILE"
    sleep 0.2
    if ! is_running; then
      echo "[FAIL] sister-praxis-http did not stay running"
      tail -50 "$LOG_FILE" || true
      exit 4
    fi
    echo "[PASS] sister-praxis-http started pid=$(cat "$PID_FILE") bind=$BIND port=$PORT"
    ;;
  stop)
    if ! is_running; then
      rm -f "$PID_FILE"
      echo "[PASS] sister-praxis-http already stopped"
      exit 0
    fi
    pid="$(cat "$PID_FILE")"
    kill "$pid"
    for _ in {1..20}; do
      kill -0 "$pid" 2>/dev/null || break
      sleep 0.1
    done
    if kill -0 "$pid" 2>/dev/null; then
      echo "[FAIL] process did not stop: $pid"
      exit 5
    fi
    rm -f "$PID_FILE"
    echo "[PASS] sister-praxis-http stopped"
    ;;
  status)
    if is_running; then
      echo "[UP] sister-praxis-http pid=$(cat "$PID_FILE") bind=$BIND port=$PORT"
    else
      echo "[DOWN] sister-praxis-http"
      exit 1
    fi
    ;;
  health)
    curl --fail --silent --show-error \
      "http://${BIND}:${PORT}/health"
    printf '\n'
    ;;
  restart)
    "$0" stop || true
    "$0" start
    ;;
  *)
    echo "usage: $0 start|stop|restart|status|health"
    exit 64
    ;;
esac
