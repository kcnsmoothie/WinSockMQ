#pragma once
#include <string>
#include <queue>
#include <vector>
#include <winsock2.h>

struct StoredMessage {
    std::string exchange;
    std::string routingKey;
    std::string message;
};

extern std::vector<StoredMessage> messages;
extern std::queue<std::string> animal;
extern std::queue<std::string> fruit;

int recv_all(SOCKET sock, char* buf, int len);
std::string recv_string(SOCKET sock);
void parseRoutingKey();