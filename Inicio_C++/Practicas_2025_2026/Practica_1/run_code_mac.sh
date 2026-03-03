#!/usr/bin/env bash
set -euo pipefail

DURATION="${DURATION:-10}"
OUT="${OUT:-mqtt_capture.log}"
BIN="${BIN:-./mqtt_subscribe_emqx}"

if [[ ! -x "$BIN" ]]; then
  echo "[ERR] No encuentro ejecutable o no es ejecutable: $BIN"
  exit 1
fi

echo "[1/4] Ejecutando $BIN y guardando salida en $OUT durante ${DURATION}s..."
: > "$OUT"

# --- Captura sin pipeline: FIFO para poder matar el PID real del BIN ---
TMPDIR="${TMPDIR:-/tmp}"
FIFO="$(mktemp -u "$TMPDIR/mqtt_fifo.XXXXXX")"
cleanup() { rm -f "$FIFO"; }
trap cleanup EXIT

mkfifo "$FIFO"

# lector: escribe FIFO a terminal y archivo
tee -a "$OUT" < "$FIFO" &
TEE_PID=$!

# escritor: tu binario escribe a FIFO
"$BIN" > "$FIFO" 2>&1 &
BIN_PID=$!

echo "[INFO] BIN_PID=$BIN_PID (mqtt_subscribe_emqx) | TEE_PID=$TEE_PID (tee)"

sleep "$DURATION" || true

echo
echo "[2/4] Deteniendo BIN (pid=$BIN_PID)..."

# paro amable -> duro, pero sobre el PID REAL del binario
if kill -0 "$BIN_PID" 2>/dev/null; then
  kill -INT "$BIN_PID" 2>/dev/null || true
  sleep 0.5
fi
if kill -0 "$BIN_PID" 2>/dev/null; then
  kill -TERM "$BIN_PID" 2>/dev/null || true
  sleep 0.5
fi
if kill -0 "$BIN_PID" 2>/dev/null; then
  kill -KILL "$BIN_PID" 2>/dev/null || true
fi

# esperar al binario y cerrar FIFO; luego matar tee si sigue
wait "$BIN_PID" 2>/dev/null || true

# tee puede quedar esperando EOF; le damos un momento y si no, lo terminamos
sleep 0.2
if kill -0 "$TEE_PID" 2>/dev/null; then
  kill -TERM "$TEE_PID" 2>/dev/null || true
fi
wait "$TEE_PID" 2>/dev/null || true

echo "[3/4] Parseando y graficando..."
# (aquí va tu bloque python tal cual)

echo "[4/4] Listo ✅"
echo "Log guardado en: $OUT"
