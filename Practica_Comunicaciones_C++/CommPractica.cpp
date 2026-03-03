#include <iostream>
#include <string>
#include <mqtt/async_client.h>

const std::string SERVER_ADDRESS("tcp://broker.hivemq.com:1883"); // Servidor MQTT (puedes cambiarlcd ..

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



