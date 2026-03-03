#!/usr/bin/env bash
set -euo pipefail

# build_mqtt_example_macos.sh
# Compila mqtt_subscribe_emqx.cpp y deja el ejecutable en la MISMA carpeta.
#
# Uso:
#   chmod +x build_mqtt_example_macos.sh
#   ./build_mqtt_example_macos.sh
#
# Opcional:
#   SRC=otro.cpp OUT=mi_bin ./build_mqtt_example_macos.sh
#   SSL=0 ./build_mqtt_example_macos.sh   # usa paho-mqtt3a en vez de paho-mqtt3as

SRC="${SRC:-mqtt_subscribe_emqx.cpp}"
OUT="${OUT:-mqtt_subscribe_emqx}"
SSL="${SSL:-1}"

if ! command -v brew >/dev/null 2>&1; then
  echo "[ERR] Homebrew no está en PATH."
  exit 1
fi

BREW_PREFIX="$(brew --prefix)"
if [[ -z "$BREW_PREFIX" ]]; then
  echo "[ERR] brew --prefix devolvió vacío."
  exit 1
fi

PREFIX="${PREFIX_OVERRIDE:-$BREW_PREFIX}"

# libs: con SSL (paho-mqtt3as) o sin SSL (paho-mqtt3a)
LIB_C="paho-mqtt3a"
if [[ "$SSL" == "1" ]]; then
  LIB_C="paho-mqtt3as"
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

if [[ ! -f "$SRC" ]]; then
  echo "[ERR] No encuentro $SRC en $SCRIPT_DIR"
  exit 1
fi

echo "[1/3] Compilando en: $SCRIPT_DIR"
echo "      PREFIX=$PREFIX"
echo "      SRC=$SRC"
echo "      OUT=$OUT"
echo "      LIB_C=$LIB_C"

clang++ -std=c++17 "$SRC" -o "$OUT" \
  -I"$PREFIX/include" -L"$PREFIX/lib" \
  -Wl,-rpath,"$PREFIX/lib" \
  -lpaho-mqttpp3 -l"$LIB_C" -pthread

echo "[2/3] OK ✅ Ejecutable creado: $SCRIPT_DIR/$OUT"

echo "[3/3] Dependencias (otool -L):"
otool -L "$OUT" | sed 's/^/  /'
