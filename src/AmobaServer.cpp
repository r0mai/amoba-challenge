
#include <iostream>
#include "AmobaServer.hpp"
#include "Map.hpp"

void AmobaServer::addClient(ClientPtr client) {
    clients.push_back(client);
}

void AmobaServer::run(int xSize, int ySize) {
    Map map(xSize, std::vector<int>(ySize, 0));

    int clientIndex = 0;
    while (true) {
        printMap(map);

        int winner = hasNInARow(map, clients.size() + 3);

        if (winner != 0) {
            std::cout << "Player " << winner << " won" << std::endl;
            return;
        }
        if (!hasEmpty(map)) {
            std::cout << "Map is full. Draw" << std::endl;
            return;
        }

        auto position =
            clients[clientIndex]->getMove(map, clientIndex+1, clients.size());

        int x = std::get<0>(position);
        int y = std::get<1>(position);

        if (x < 0 || y < 0 || x >= map.size() || y >= map[0].size()) {
            std::cout << "Out of bounds" << std::endl;
            return;
        }

        if (map[x][y] != 0) {
            std::cout << "Bad move" << std::endl;
            return;
        }

        map[x][y] = clientIndex+1;

        clientIndex += 1;
        clientIndex %= clients.size();
    }
}

