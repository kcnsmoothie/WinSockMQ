#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "handler.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4882);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "Exchange Server running..." << std::endl;

    acceptLoop(serverSocket);

    closesocket(serverSocket);
    WSACleanup();
}

