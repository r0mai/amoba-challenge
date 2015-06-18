
#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include "AmobaServer.hpp"
#include "Map.hpp"

void AmobaServer::addClient(ClientPtr client) {
    clients.push_back(client);
}

void AmobaServer::run(bool isInfinite, int xSize, int ySize) {
    std::unique_ptr<Map> map;
    if (isInfinite)
        map = std::make_unique<InfiniteMap>(clients.size() + 3);
    else
        map = std::make_unique<BoundedMap>(xSize, ySize);

    static std::random_device rd;
    static std::mt19937 g(rd());

    std::shuffle(clients.begin(), clients.end(), g);

    int clientIndex = 0;
    while (true) {
        printMap(*map);

        int winner = hasNInARow(*map, clients.size() + 3);

        if (winner != 0) {
            std::cout << "Player " << winner << " (aka. " << playerToChar(winner) << ") won (" << clients[winner - 1]->getName() << ")" << std::endl;
            return;
        }
        if (!hasEmpty(*map)) {
            std::cout << "Map is full. Draw" << std::endl;
            return;
        }

        auto position =
            clients[clientIndex]->getMove(*map, clientIndex+1, clients.size());

        int x = std::get<0>(position);
        int y = std::get<1>(position);

        if (!map->valid(x, y)) {
            std::cout << "Out of bounds" << std::endl;
            return;
        }

        if (map->get(x, y) != 0) {
            std::cout << "Bad move" << std::endl;
            return;
        }

        map->set(x, y, clientIndex + 1);

        clientIndex += 1;
        clientIndex %= clients.size();
    }
}

