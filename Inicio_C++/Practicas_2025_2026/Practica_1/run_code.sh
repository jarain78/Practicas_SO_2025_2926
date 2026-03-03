#!/usr/bin/env bash
# ↑ Shebang: ejecuta este script con bash (no sh). En macOS suele estar en /usr/bin/env bash.

set -euo pipefail
# -e : si un comando falla (exit != 0), el script termina
# -u : si usas una variable no definida, termina (evita bugs)
# -o pipefail : si falla un comando dentro de un pipe, el pipe entero cuenta como fallo

# ============================================================
# capture_plot_mqtt.sh
# Ejecuta mqtt_subscribe_emqx, captura salida por terminal,
# la guarda en un archivo, detiene tras N segundos,
# y grafica valores numéricos detectados en "Payload:".
# ============================================================

# --------- Parámetros configurables por variables de entorno ---------
DURATION="${DURATION:-10}"             # segundos de captura (default: 10)
OUT="${OUT:-mqtt_capture.log}"         # archivo donde se guarda la salida (default)
BIN="${BIN:-./mqtt_subscribe_emqx}"    # ejecutable MQTT (default: en la carpeta actual)

# --------- Validación del ejecutable ---------
if [[ ! -x "$BIN" ]]; then                      # -x => existe y es ejecutable
  echo "[ERR] No encuentro ejecutable o no es ejecutable: $BIN"
  echo "Tip: chmod +x $BIN"
  exit 1
fi

# --------- Arranque y captura de logs ---------
echo "[1/4] Ejecutando $BIN y guardando salida en $OUT durante ${DURATION}s..."

: > "$OUT"
# ":" es un comando no-op. Con redirección "> $OUT" vacía/crea el archivo.

# Ejecutar en background y capturar stdout/stderr
"$BIN" 2>&1 | tee -a "$OUT" &
# 2>&1       => redirige stderr a stdout
# tee -a     => muestra por terminal y además agrega (-a) al archivo OUT
# &          => lo manda a background para que el script siga

PID=$!
# $! guarda el PID (process id) del último proceso lanzado en background (el pipe completo).

# --------- Espera de captura ---------
sleep "$DURATION" || true
# Espera N segundos. "|| true" evita que un posible error de sleep pare el script.

echo
echo "[2/4] Deteniendo proceso (pid=$PID)..."

# --------- Parada del proceso: intento suave -> más duro ---------
# Nota: aquí había un bug en tu versión: estabas usando kill -9 en las condiciones.
# kill -9 mata inmediatamente y no permite cierre limpio.
# Lo correcto: comprobar si el proceso existe con kill -0.

if kill -0 "$PID" 2>/dev/null; then
  # kill -0 no mata: solo verifica si el proceso existe y tienes permiso
  kill -INT "$PID" 2>/dev/null || true        # Ctrl+C (SIGINT) para cierre "amable"
  sleep 0.5
fi

if kill -0 "$PID" 2>/dev/null; then
  kill -TERM "$PID" 2>/dev/null || true       # SIGTERM: fin "normal"
  sleep 0.5
fi

if kill -0 "$PID" 2>/dev/null; then
  kill -KILL "$PID" 2>/dev/null || true       # SIGKILL: fuerza bruta si no respondió
fi

# Evitar que el shell se quede con el job colgando (y limpiar el estado del proceso)
wait "$PID" 2>/dev/null || true
# wait recoge el estado de salida del proceso background para que no quede "zombie".
# 2>/dev/null oculta mensajes si ya murió o no existe.

echo "[3/4] Parseando datos numéricos desde '$OUT' y graficando en terminal..."

# ============================================================
# Bloque Python embebido: parsea el OUT y dibuja gráfica ASCII
# ============================================================
python3 - <<'PY'
import os
import re
import sys
import json
from pathlib import Path

import matplotlib.pyplot as plt


LOG = Path(os.environ.get("OUT", "mqtt_capture.log"))
OUT_DIR = Path(os.environ.get("PLOT_DIR", "."))
OUT_DIR.mkdir(parents=True, exist_ok=True)

TOPIC_GAS = os.environ.get("TOPIC_GAS", "sensor/data/gas_sensor")
TOPIC_SEN55 = os.environ.get("TOPIC_SEN55", "sensor/data/sen55")

if not LOG.exists():
    print(f"[ERR] No existe el archivo: {LOG}", file=sys.stderr)
    sys.exit(1)

lines = LOG.read_text(errors="ignore").splitlines()

topic_re = re.compile(r"^\[MSG\]\s+Topic:\s*(.+)\s*$")
payload_re = re.compile(r"^\s*Payload:\s*(.+)\s*$")

last_payload = {}  # topic -> dict (último JSON dict válido)
current_topic = None

for line in lines:
    mt = topic_re.match(line)
    if mt:
        current_topic = mt.group(1).strip()
        continue

    mp = payload_re.match(line)
    if mp and current_topic:
        topic = current_topic
        current_topic = None

        payload_str = mp.group(1).strip()
        try:
            obj = json.loads(payload_str)
        except Exception:
            continue

        if isinstance(obj, dict):
            # nos quedamos con el último dict válido por tópico
            last_payload[topic] = obj


def plot_bar(payload: dict, title: str, out_file: Path):
    # solo valores numéricos
    keys = []
    vals = []
    for k, v in payload.items():
        try:
            vals.append(float(v))
            keys.append(str(k))
        except Exception:
            pass

    if not keys:
        print(f"[WARN] '{title}': no hay valores numéricos para plotear.")
        return

    plt.figure(figsize=(11, 4))
    plt.bar(range(len(keys)), vals)
    plt.xticks(range(len(keys)), keys, rotation=35, ha="right")
    plt.title(title)
    plt.ylabel("Valor")
    plt.grid(True, axis="y", alpha=0.3)
    plt.tight_layout()
    plt.savefig(out_file, dpi=200)
    plt.close()
    print(f"[OK] Guardado: {out_file.resolve()}")


# Tomar último payload de cada tópico deseado
#gas = last_payload.get(TOPIC_GAS)
#sen55 = last_payload.get(TOPIC_SEN55)

if gas is None and sen55 is None:
    print("[ERR] No encontré payloads JSON para los tópicos esperados en el log.")
    print(f"      TOPIC_GAS={TOPIC_GAS}")
    print(f"      TOPIC_SEN55={TOPIC_SEN55}")
    print("\nTópicos encontrados en el log:")
    for t in sorted(last_payload.keys()):
        print(f" - {t}")
    sys.exit(1)

if gas is not None:
    plot_bar(gas, f"{TOPIC_GAS} (último payload)", OUT_DIR / "gas_sensor_snapshot.png")
else:
    print(f"[WARN] No encontré datos para {TOPIC_GAS}")

if sen55 is not None:
    plot_bar(sen55, f"{TOPIC_SEN55} (último payload)", OUT_DIR / "sen55_snapshot.png")
else:
    print(f"[WARN] No encontré datos para {TOPIC_SEN55}")
PY

echo "[4/4] Listo ✅"
echo "Log guardado en: $OUT"

kill -9 "$PID" 
# ============================================================
# AYUDA RÁPIDA / USO
#
# 1) Ejecutar 10s (por defecto):
#    ./capture_plot_mqtt.sh
#
# 2) Cambiar duración a 30s:
#    DURATION=30 ./capture_plot_mqtt.sh
#
# 3) Cambiar archivo de salida:
#    OUT=mi_log.txt ./capture_plot_mqtt.sh
#
# 4) Cambiar ejecutable MQTT:
#    BIN=./otro_subscriptor ./capture_plot_mqtt.sh
#
# 5) IMPORTANTE:
#    El parser extrae el PRIMER número que encuentre en cada "Payload:".
#    Si tus payloads son JSON, lo ideal es mandar algo tipo:
#      {"value": 12.34}
#    o directamente:
#      12.34

# ============================================================
