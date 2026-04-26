// mqtt_subscribe_emqx_sin_clases.cpp

#include <mqtt/async_client.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const std::string BROKER = "tcp://broker.emqx.io:1883";
    const std::string CLIENT_ID = "cpp_sub_" + std::to_string(std::rand());

    const std::vector<std::string> TOPICS = {
        "sensor/data/sen55",
        "sensor/data/gas_sensor"
    };

    mqtt::async_client client(BROKER, CLIENT_ID);

    mqtt::connect_options connopts;
    connopts.set_clean_session(true);
    connopts.set_keep_alive_interval(20);
    connopts.set_automatic_reconnect(true);

    try {
        std::cout << "[MQTT] Conectando a " << BROKER
                  << " con client_id=" << CLIENT_ID << "...\n";

        client.connect(connopts)->wait();

        std::cout << "[MQTT] Conectado correctamente.\n";

        client.start_consuming();

        for (const auto& topic : TOPICS) {
            std::cout << "[MQTT] Suscribiendo a: " << topic << "\n";

            auto tok = client.subscribe(topic, 0);

            if (!tok->wait_for(std::chrono::seconds(5))) {
                std::cerr << "[MQTT] ERROR: timeout al suscribirse a "
                          << topic << "\n";
            } else {
                std::cout << "[MQTT] Suscrito correctamente a: "
                          << topic << "\n";
            }
        }

        std::cout << "[MQTT] Escuchando mensajes...\n";
        std::cout << "Pulsa Ctrl+C para finalizar.\n\n";

        while (true) {
            mqtt::const_message_ptr msg;

            bool received = client.try_consume_message_for(
                &msg,
                std::chrono::milliseconds(500)
            );

            if (received && msg) {
                std::cout << "\n[MSG] Topic: "
                          << msg->get_topic()
                          << "\n      Payload: "
                          << msg->to_string()
                          << "\n";
            }
        }

        client.stop_consuming();

        for (const auto& topic : TOPICS) {
            client.unsubscribe(topic)->wait();
        }

        client.disconnect()->wait();

        std::cout << "[MQTT] Desconectado.\n";

        return 0;
    }
    catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Excepción: " << e.what() << "\n";
        return 1;
    }
}