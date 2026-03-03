#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>

int main() {
    const char* hostname = "example.com";
    const char* port = "80";
    const char* path = "/login";

    std::string body = "username=juan&password=1234";

    // Construcción de la solicitud POST
    std::string request =
        "POST " + std::string(path) + " HTTP/1.1\r\n" +
        "Host: " + hostname + "\r\n" +
        "Content-Type: application/x-www-form-urlencoded\r\n" +
        "Content-Length: " + std::to_string(body.length()) + "\r\n" +
        "Connection: close\r\n\r\n" +
        body;

    // Resolución del servidor
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(hostname, port, &hints, &res);
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sock, res->ai_addr, res->ai_addrlen);

    // Enviar solicitud
    send(sock, request.c_str(), request.length(), 0);

    // Leer respuesta
    char buffer[4096];
    int bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << buffer;
    }

    // Cerrar socket
    close(sock);
    freeaddrinfo(res);

    return 0;
}
