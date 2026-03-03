// mqtt_subscribe_emqx.cpp
#include <mqtt/async_client.h>

#include <atomic>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

static std::atomic<bool> g_running{true};
static void on_signal(int) { g_running.store(false); }

class Callback final : public virtual mqtt::callback {
public:
    void connected(const std::string& cause) override {
        std::cout << "[MQTT] Conectado. Cause: " << (cause.empty() ? "(none)" : cause) << "\n";
        // ✅ NO suscribir aquí (evita deadlock con wait() en callback)
    }

    void connection_lost(const std::string& cause) override {
        std::cerr << "[MQTT] Conexión perdida. Cause: " << (cause.empty() ? "(none)" : cause) << "\n";
    }

    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout << "\n[MSG] Topic: " << msg->get_topic()
                  << "\n      Payload: " << msg->to_string()
                  << "\n";
    }
};

static bool subscribe_with_timeout(mqtt::async_client& cli, const std::string& topic, int qos,
                                  std::chrono::seconds timeout = std::chrono::seconds(5)) {
    std::cout << "[MQTT] Subscribing: " << topic << " (qos=" << qos << ")...\n";
    auto tok = cli.subscribe(topic, qos);
    if (!tok->wait_for(timeout)) {
        std::cerr << "[MQTT] ERROR: subscribe timeout en " << topic << "\n";
        return false;
    }
    std::cout << "[MQTT] OK subscribed: " << topic << "\n";
    return true;
}

static void unsubscribe_best_effort(mqtt::async_client& cli, const std::string& topic) {
    try {
        std::cout << "[MQTT] Unsubscribe: " << topic << "\n";
        auto tok = cli.unsubscribe(topic);
        tok->wait_for(std::chrono::seconds(3));
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] WARN unsubscribe " << topic << " -> " << e.what() << "\n";
    }
}

int main() {
    std::signal(SIGINT, on_signal);
    std::signal(SIGTERM, on_signal);
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

    Callback cb;
    client.set_callback(cb);

    try {
        std::cout << "[MQTT] Conectando a " << BROKER << " con client_id=" << CLIENT_ID << "...\n";
        client.connect(connopts)->wait();
        // ✅ Suscribir SOLO aquí
        for (const auto& t : TOPICS) {
            subscribe_with_timeout(client, t, 0);
        }

        std::cout << "[MQTT] Escuchando... (Ctrl+C para salir)\n";
        while (g_running.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        std::cout << "\n[MQTT] Cerrando...\n";
        for (const auto& t : TOPICS) {
            unsubscribe_best_effort(client, t);
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
