#!/usr/bin/env bash
set -euo pipefail

# install_paho_mqtt_macos.sh
# Instala dependencias + Paho MQTT C (brew) + Paho MQTT C++ (build from source)
#
# Uso:
#   chmod +x install_paho_mqtt_macos.sh
#   ./install_paho_mqtt_macos.sh
#
# Opcional:
#   PAHO_CPP_REF=v1.5.3 PREFIX_OVERRIDE="$(brew --prefix)" ./install_paho_mqtt_macos.sh

PAHO_CPP_REF="${PAHO_CPP_REF:-v1.4.1}"   # tag/branch/commit paho.mqtt.cpp
WORKDIR="${WORKDIR:-$HOME/paho_build_macos}"

# Prefijo de brew: /opt/homebrew (Apple Silicon) o /usr/local (Intel)
BREW_PREFIX="$(brew --prefix 2>/dev/null || true)"
PREFIX="${PREFIX_OVERRIDE:-$BREW_PREFIX}"

if [[ -z "${BREW_PREFIX}" ]]; then
  echo "[ERR] Homebrew no está instalado o no está en PATH."
  echo "Instálalo desde https://brew.sh y vuelve a ejecutar."
  exit 1
fi

echo "[1/7] Instalando herramientas base (cmake, git, openssl)..."
brew update
brew install cmake git openssl@3 || true

echo "[2/7] Instalando Paho MQTT C (libpaho-mqtt) via Homebrew..."
brew install libpaho-mqtt || true

echo "[3/7] Preparando directorio de build: $WORKDIR"
mkdir -p "$WORKDIR"
cd "$WORKDIR"

echo "[4/7] Clonando/actualizando paho.mqtt.cpp ($PAHO_CPP_REF)..."
if [[ -d paho.mqtt.cpp ]]; then
  cd paho.mqtt.cpp
  git fetch --all --tags
else
  git clone https://github.com/eclipse-paho/paho.mqtt.cpp.git
  cd paho.mqtt.cpp
fi

git checkout "$PAHO_CPP_REF"

echo "[5/7] Configurando CMake para paho.mqtt.cpp..."
# SSL: OpenSSL de brew
OPENSSL_PREFIX="$(brew --prefix openssl@3)"

rm -rf build
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DPAHO_BUILD_SHARED=ON \
  -DPAHO_BUILD_STATIC=OFF \
  -DPAHO_WITH_SSL=ON \
  -DOPENSSL_ROOT_DIR="$OPENSSL_PREFIX" \
  -DOPENSSL_INCLUDE_DIR="$OPENSSL_PREFIX/include" \
  -DOPENSSL_SSL_LIBRARY="$OPENSSL_PREFIX/lib/libssl.dylib" \
  -DOPENSSL_CRYPTO_LIBRARY="$OPENSSL_PREFIX/lib/libcrypto.dylib" \
  -DCMAKE_PREFIX_PATH="$PREFIX" \
  -DCMAKE_INSTALL_PREFIX="$PREFIX"

echo "[6/7] Compilando e instalando paho.mqtt.cpp en $PREFIX ..."
cmake --build build -j"$(sysctl -n hw.ncpu)"
cmake --install build

echo "[7/7] Verificación rápida..."
echo "  - Headers mqtt:"
ls -1 "$PREFIX/include" | grep -E "^mqtt$" || true
echo "  - Librerías paho:"
ls -1 "$PREFIX/lib" | grep -E "paho" || true

cat <<EOF

OK ✅

Para compilar tu mqtt_subscribe_emqx.cpp en macOS (clang++ recomendado):
  clang++ -std=c++17 mqtt_subscribe_emqx.cpp -o mqtt_subscribe_emqx \\
    -I"$PREFIX/include" -L"$PREFIX/lib" \\
    -Wl,-rpath,"$PREFIX/lib" \\
    -lpaho-mqttpp3 -lpaho-mqtt3as -pthread

Notas:
- Si NO usas SSL, cambia -lpaho-mqtt3as por -lpaho-mqtt3a.
- Si el linker no encuentra OpenSSL, añade:
    -I"$(brew --prefix openssl@3)/include" -L"$(brew --prefix openssl@3)/lib"

EOF
