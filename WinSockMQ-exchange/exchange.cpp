#include <iostream>
#include <queue>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <queue>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct StoredMessage {
    string exchange;
    string routingKey;
    string message;
};

vector<StoredMessage> messages;

int recv_all(SOCKET sock, char* buf, int len) {
    int total = 0;
    while (total < len) {
        int received = recv(sock, buf + total, len - total, 0);
        if (received <= 0) return -1;
        total += received;
    }
    return total;
}

string recv_string(SOCKET sock) {
    uint32_t len_raw;
    if (recv_all(sock, (char*)&len_raw, 4) <= 0) return "";
    uint32_t len = ntohl(len_raw);

    string out(len, 0);
    if (recv_all(sock, &out[0], len) <= 0) return "";

    return out;
}

queue<string> animal;
queue<string> fruit;

void parseRoutingKey() {
    for (int i = 0; i < messages.size(); i++) {
        string routingKey = messages[i].routingKey;

        if (routingKey.rfind("animal", 0) == 0) {
            animal.push(messages[i].message);
            cout << "[animal 큐에 추가됨] " << messages[i].message << endl;
        }
        else if (routingKey.rfind("fruit", 0) == 0) {
            animal.push(messages[i].message);
            cout << "[fruit 큐에 추가됨] " << messages[i].message << endl;
        }
    }
}

void handleProducer(SOCKET client) {
    while (true) {
        string exchange = recv_string(client);
        if (exchange.empty()) break;

        string routingKey = recv_string(client);
        if (routingKey.empty()) break;

        string message = recv_string(client);
        if (message.empty()) break;

        messages.push_back({ exchange, routingKey, message });

        cout << "\n--- 메시지 저장됨 (순서 " << messages.size() << ") ---" << endl;
        cout << "Exchange   : " << exchange << endl;
        cout << "RoutingKey : " << routingKey << endl;
        cout << "Message    : " << message << endl;

        parseRoutingKey();
    }

    closesocket(client);
    cout << "Producer disconnected.\n" << endl;
}

void acceptLoop(SOCKET serverSocket) {
    while (true) {
        sockaddr_in clientAddr{};
        int clientSize = sizeof(clientAddr);

        SOCKET client = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
        if (client == INVALID_SOCKET) {
            cout << "accept failed, error: " << WSAGetLastError() << endl;
            continue;
        }

        cout << "\nProducer connected." << endl;
        handleProducer(client);
    }
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "WSAStartup failed\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cout << "socket() failed, error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4882);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (bind(serverSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Bind failed, error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Listen failed, error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Exchange Server running on 127.0.0.1:4882" << endl;

    acceptLoop(serverSocket);

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
