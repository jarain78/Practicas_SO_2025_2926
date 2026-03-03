#include "RobotControl.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

void initializeSockets() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

void cleanupSockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}

void closeSocket(int sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

int createSocketAndConnect(sockaddr_in &server) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "❌ Error creando socket\\n";
        return -1;
    }

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "❌ Error conectando con el servidor\\n";
        closeSocket(sock);
        return -1;
    }

    return sock;
}

void sendPosition(float x, float y, float z) {
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    int sock = createSocketAndConnect(server);
    if (sock < 0) return;

    std::ostringstream json;
    json << "{\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << "}";
    std::string body = json.str();

    std::ostringstream oss;
    oss << "POST /add_obstacle HTTP/1.1\\r\\n"
        << "Host: localhost:8080\\r\\n"
        << "Content-Type: application/json\\r\\n"
        << "Content-Length: " << body.length() << "\\r\\n"
        << "Connection: close\\r\\n\\r\\n"
        << body;

    std::string request = oss.str();
    send(sock, request.c_str(), request.length(), 0);

    char buffer[4096];
    std::ostringstream response;
    ssize_t bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        response << buffer;
    }

    std::cout << "📥 Respuesta al enviar posición:\\n" << response.str() << std::endl;
    closeSocket(sock);
}

void getCurrentPosition() {
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    int sock = createSocketAndConnect(server);
    if (sock < 0) return;

    std::string request =
        "GET /position HTTP/1.1\\r\\n"
        "Host: localhost:8080\\r\\n"
        "Connection: close\\r\\n\\r\\n";

    send(sock, request.c_str(), request.length(), 0);

    char buffer[4096];
    std::ostringstream response;
    ssize_t bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        response << buffer;
    }

    std::cout << "📥 Posición actual recibida:\\n" << response.str() << std::endl;
    closeSocket(sock);
}

void sendPath() {
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    int sock = createSocketAndConnect(server);
    if (sock < 0) return;
    std::string body = "{"
    "\"points\": ["
    "{\"x\": 0, \"y\": 0, \"z\": 0},"
    "{\"x\": 2, \"y\": 0, \"z\": 2},"
    "{\"x\": 4, \"y\": 0, \"z\": 0},"
    "{\"x\": 6, \"y\": 0, \"z\": -2}"
    "]"
"}";

      
      
    std::ostringstream oss;
    oss << "POST / HTTP/1.1\\r\\n"
        << "Host: localhost:8080\\r\\n"
        << "Content-Type: application/json\\r\\n"
        << "Content-Length: " << body.length() << "\\r\\n"
        << "Connection: close\\r\\n\\r\\n"
        << body;

    std::string request = oss.str();
    send(sock, request.c_str(), request.length(), 0);

    char buffer[4096];
    std::ostringstream response;
    ssize_t bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        response << buffer;
    }

    std::cout << "📥 Respuesta al enviar ruta:\\n" << response.str() << std::endl;
    closeSocket(sock);
}
