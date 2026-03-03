#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

void initializeSockets();
void cleanupSockets();
void closeSocket(int sock);
int createSocketAndConnect(sockaddr_in &server);
void sendPosition(float x, float y, float z);
void getCurrentPosition();
void sendPath();

#endif
