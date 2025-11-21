#include "message.h"
#include <iostream>
#include <thread>

void handleConsumer(SOCKET client) {
    while (true) {
        std::string queueName = recv_string(client);
        if (queueName.empty()) break;

        std::string message;
        if (queueName == "animal" && !animal.empty()) {
            message = animal.front();
            animal.pop();
        }
        else if (queueName == "fruit" && !fruit.empty()) {
            message = fruit.front();
            fruit.pop();
        }
        else {
            message = "";
        }

        uint32_t len = htonl(message.size());
        send(client, (char*)&len, 4, 0);

        if (!message.empty()) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
    closesocket(client);
}

void handleProducer(SOCKET client) {
    while (true) {
        std::string exchange = recv_string(client);
        if (exchange.empty()) break;

        std::string routingKey = recv_string(client);
        if (routingKey.empty()) break;

        std::string message = recv_string(client);
        if (message.empty()) break;

        messages.push_back({ exchange, routingKey, message });
        parseRoutingKey();
    }
    closesocket(client);
}

void acceptLoop(SOCKET serverSocket) {
    while (true) {
        sockaddr_in clientAddr{};
        int clientSize = sizeof(clientAddr);

        SOCKET client = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
        if (client == INVALID_SOCKET) continue;

        char role;
        int r = recv(client, &role, 1, 0);
        if (r <= 0) {
            closesocket(client);
            continue;
        }

        if (role == 'P') {
            std::cout << "[Producer connected]" << std::endl;
            std::thread t(handleProducer, client);
            t.detach();
        }
        else if (role == 'C') {
            std::cout << "[Consumer connected]" << std::endl;
            std::thread t(handleConsumer, client);
            t.detach();
        }
        else {
            std::cout << "[Unknown role. Closing connection]\n";
            closesocket(client);
        }

        std::thread t1(handleProducer, client);

        t1.detach();
    }
}