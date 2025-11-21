#include "message.h"
#include <iostream>

std::vector<StoredMessage> messages;
std::queue<std::string> animal;
std::queue<std::string> fruit;

int recv_all(SOCKET sock, char* buf, int len) {
    int total = 0;
    while (total < len) {
        int received = recv(sock, buf + total, len - total, 0);
        if (received <= 0) return -1;
        total += received;
    }
    return total;
}

std::string recv_string(SOCKET sock) {
    uint32_t len_raw;
    if (recv_all(sock, (char*)&len_raw, 4) <= 0) return "";
    uint32_t len = ntohl(len_raw);

    std::string out(len, 0);
    if (recv_all(sock, &out[0], len) <= 0) return "";

    return out;
}

void parseRoutingKey() {
    for (int i = 0; i < messages.size(); i++) {
        std::string routingKey = messages[i].routingKey;

        if (routingKey.rfind("animal", 0) == 0) {
            animal.push(messages[i].message);
            std::cout << "[animal Å¥¿¡ Ãß°¡µÊ] " << messages[i].message << std::endl;
        }
        else if (routingKey.rfind("fruit", 0) == 0) {
            fruit.push(messages[i].message);
            std::cout << "[fruit Å¥¿¡ Ãß°¡µÊ] " << messages[i].message << std::endl;
        }
    }
}
