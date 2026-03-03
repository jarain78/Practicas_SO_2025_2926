Para comunicarte con un **servidor MQTT** desde **C++ en tu PC**, puedes usar la biblioteca **Paho MQTT de Eclipse**, que es una de las más utilizadas para MQTT en C++.

---

### **Pasos para conectar un cliente MQTT en C++ a un servidor**
1. **Instalar la biblioteca Paho MQTT**
2. **Escribir un programa en C++** para publicar y suscribirse a un tópico MQTT
3. **Probar la conexión con un servidor MQTT (ejemplo: Eclipse Mosquitto)**

---

## **1. Instalación de Paho MQTT en Linux y Windows**
### **Linux (Ubuntu/Debian)**
```sh
sudo apt update
sudo apt install libpaho-mqtt-dev
```
### **Windows (vcpkg)**
```sh
vcpkg install paho-mqtt
```

---

## **2. Código en C++ para Conectar a un Servidor MQTT**
Aquí tienes un ejemplo de un **cliente MQTT en C++** que **publica** y **se suscribe** a un tópico usando **Paho MQTT**:

```cpp
#include <iostream>
#include <string>
#include <mqtt/async_client.h>

const std::string SERVER_ADDRESS("tcp://broker.hivemq.com:1883"); // Servidor MQTT (puedes cambiarlo)
const std::string CLIENT_ID("MQTT_Client_CPP");
const std::string TOPIC("test/mqtt_cpp");

class callback : public virtual mqtt::callback {
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout << "Mensaje recibido en [" << msg->get_topic() << "]: " 
                  << msg->to_string() << std::endl;
    }
};

int main() {
    try {
        mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
        callback cb;
        client.set_callback(cb);

        std::cout << "Conectando al servidor MQTT..." << std::endl;
        mqtt::connect_options connOpts;
        connOpts.set_keep_alive_interval(20);
        connOpts.set_clean_session(true);
        client.connect(connOpts)->wait();

        std::cout << "Suscribiéndose al tópico: " << TOPIC << std::endl;
        client.subscribe(TOPIC, 1);

        std::string mensaje;
        while (true) {
            std::cout << "Ingrese un mensaje para publicar (o 'exit' para salir): ";
            std::getline(std::cin, mensaje);
            if (mensaje == "exit") break;

            client.publish(TOPIC, mensaje.c_str(), mensaje.length(), 1, false);
        }

        std::cout << "Desconectando del servidor MQTT..." << std::endl;
        client.disconnect()->wait();
    } catch (const mqtt::exception& e) {
        std::cerr << "Error MQTT: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

---

## **3. Explicación del Código**
🔹 **Conexión al servidor MQTT** usando `mqtt::async_client`.  
🔹 **Suscripción** al tópico `"test/mqtt_cpp"`.  
🔹 **Publicación de mensajes** en el mismo tópico.  
🔹 **Callback** que imprime los mensajes recibidos.  
🔹 **Bucle de entrada de usuario** para enviar mensajes hasta que el usuario escriba `"exit"`.

---

## **4. Prueba con un Servidor MQTT**
Puedes probar este código con un **servidor MQTT público**, como `broker.hivemq.com`, o instalar **Mosquitto** en tu PC:

### **Instalar Mosquitto en Linux**
```sh
sudo apt install mosquitto mosquitto-clients
```

### **Ejecutar Mosquitto como servidor MQTT local**
```sh
mosquitto -v
```

Para suscribirte desde la terminal:
```sh
mosquitto_sub -h localhost -t "test/mqtt_cpp"
```

Para publicar un mensaje desde la terminal:
```sh
mosquitto_pub -h localhost -t "test/mqtt_cpp" -m "Hola desde terminal"
```

---

El error **"fatal error: 'mqtt/async_client.h' file not found"** indica que el compilador no puede encontrar la biblioteca **Paho MQTT** en tu **Mac**. Vamos a solucionarlo paso a paso.

---

## **1. Instalar la Biblioteca Paho MQTT en macOS**
En **macOS**, puedes instalar **Paho MQTT** usando **Homebrew**:

```sh
brew install eclipse-mosquitto
```
Luego, instala **Paho MQTT C y C++**:

```sh
brew install paho-mqtt3-c
brew install paho-mqtt3-cpp
```

---

## **2. Verificar la Instalación**
Verifica que los archivos de la biblioteca están instalados en:

```sh
ls /usr/local/include/mqtt/
```
Debe mostrar archivos como:
```
async_client.h
client.h
connect_options.h
```
Si no aparece, busca la ubicación con:
```sh
find /usr/local/ -name "async_client.h"
```
O, si usaste **Apple Silicon (M1/M2)**:
```sh
find /opt/homebrew/ -name "async_client.h"
```

---

## **3. Compilar el Código en macOS**
Si la biblioteca está instalada, compila tu código con:
```sh
g++ -std=c++11 -I/usr/local/include -L/usr/local/lib -lpaho-mqtt3c -lpaho-mqtt3cpp mqtt_client.cpp -o mqtt_client
```
Si usaste **Apple Silicon (M1/M2)**:
```sh
g++ -std=c++11 -I/opt/homebrew/include -L/opt/homebrew/lib -lpaho-mqtt3c -lpaho-mqtt3cpp mqtt_client.cpp -o mqtt_client
```

---

## **4. Agregar la Ruta a `CPLUS_INCLUDE_PATH` (Opcional)**
Si el compilador sigue sin encontrar `async_client.h`, puedes agregar la ruta a **CPLUS_INCLUDE_PATH**:

```sh
export CPLUS_INCLUDE_PATH=/usr/local/include:$CPLUS_INCLUDE_PATH
```
O para **Apple Silicon**:
```sh
export CPLUS_INCLUDE_PATH=/opt/homebrew/include:$CPLUS_INCLUDE_PATH
```

Luego, intenta compilar nuevamente.

---

## **5. Solución Alternativa: Instalar Paho MQTT desde el Código Fuente**
Si la instalación con `brew` no funciona, puedes compilar **Paho MQTT** manualmente:

```sh
git clone https://github.com/eclipse/paho.mqtt.cpp.git
cd paho.mqtt.cpp
cmake -Bbuild -H. -DPAHO_WITH_SSL=OFF -DPAHO_BUILD_SAMPLES=ON
cmake --build build/
sudo cmake --install build/
```

---
