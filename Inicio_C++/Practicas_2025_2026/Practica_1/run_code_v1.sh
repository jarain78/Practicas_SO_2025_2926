#!/usr/bin/env bash
set -euo pipefail

echo "Indique el tiempo de captura en segundos (default: 10):"
read -r DURATION
DURATION="${DURATION:-10}"

OUT="${OUT:-mqtt_capture.log}"
BIN="${BIN:-./mqtt_subscribe_emqx}"

if [[ ! -x "$BIN" ]]; then
  echo "[ERR] No encuentro ejecutable o no es ejecutable: $BIN"
  exit 1
fi

echo "[1/4] Ejecutando $BIN y guardando salida en $OUT durante ${DURATION}s..."
: > "$OUT"  # vacía/crea el log

# Lanzar UNA vez en background y capturar su PID real
"$BIN" >> "$OUT" 2>&1 &
PID=$!
echo "[INFO] BIN_PID=$PID"

# Esperar DURATION segundos
sleep "$DURATION" || true

echo
echo "[2/4] Deteniendo proceso (pid=$PID)..."

# Parada amable -> escalado
if kill -0 "$PID" 2>/dev/null; then
  kill -INT "$PID" 2>/dev/null || true
  sleep 0.5
fi
if kill -0 "$PID" 2>/dev/null; then
  kill -TERM "$PID" 2>/dev/null || true
  sleep 0.5
fi
if kill -0 "$PID" 2>/dev/null; then
  kill -KILL "$PID" 2>/dev/null || true
fi

# Esperar a que termine (solo funciona con procesos hijos; aquí sí lo es)
wait "$PID" 2>/dev/null || true

echo "[3/4] Parseando datos desde '$OUT' y graficando en terminal..."

python3 - <<'PY'
import re, os
from pathlib import Path

out = Path(os.environ.get("OUT","mqtt_capture.log"))
text = out.read_text(errors="ignore").splitlines()

payload_re = re.compile(r"Payload:\s*(.*)")
num_re = re.compile(r"[-+]?(?:\d+\.\d+|\d+\.|\.\d+|\d+)(?:[eE][-+]?\d+)?")

vals=[]
for line in text:
    m=payload_re.search(line)
    if not m: 
        continue
    nm=num_re.search(m.group(1))
    if nm:
        try: vals.append(float(nm.group(0)))
        except: pass

if not vals:
    print("[WARN] No se encontraron números en Payload:.")
    raise SystemExit(0)

w,h=80,20
ymin,ymax=min(vals),max(vals)
if abs(ymax-ymin)<1e-12: ymax=ymin+1.0
n=len(vals)

def xcol(i): return 0 if n==1 else int(round(i*(w-1)/(n-1)))
def yrow(y):
    t=(y-ymin)/(ymax-ymin)
    return int(round((1-t)*(h-1)))

grid=[[" "]*w for _ in range(h)]
for i,y in enumerate(vals):
    r,c=yrow(y),xcol(i)
    if 0<=r<h and 0<=c<w: grid[r][c]="*"

print("\n=== MQTT Payload Plot (ASCII) ===")
print(f"muestras={n} | y_min={ymin:.6g} | y_max={ymax:.6g} | archivo={out}\n")
for r in range(h):
    if r==0: lab=f"{ymax:>10.4g} | "
    elif r==h-1: lab=f"{ymin:>10.4g} | "
    else: lab=" "*12+"| "
    print(lab+"".join(grid[r]))
print(" "*12+"+-"+"-"*w)
print(" "*14+"0"+" "*(w-3)+f"{n-1}")
print("\nÚltimos valores:", ", ".join(f"{v:.6g}" for v in (vals[-10:] if n>10 else vals)))
PY

echo "[4/4] Listo ✅"
echo "Log guardado en: $OUT"
