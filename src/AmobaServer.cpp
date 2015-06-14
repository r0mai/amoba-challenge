
#include <iostream>
#include "AmobaServer.hpp"

namespace {

int hasNInARow(const AmobaClient::Map& map, int n) {
    int w = map.size();
    int h = map[0].size();

    for (int x = 0; x < w - n; ++x) {
        for (int y = 0; y < h; ++y) {
            int origin = map[x][y];
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map[x+i][y] != origin) {
                    break;
                }
            }
            if (i == n) {
                return origin;
            }
        }
    }

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h - n; ++y) {
            int origin = map[x][y];
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map[x][y+i] != origin) {
                    break;
                }
            }
            if (i == n) {
                return origin;
            }
        }
    }

    for (int x = 0; x < w - n; ++x) {
        for (int y = 0; y < h - n; ++y) {
            int origin = map[x][y];
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map[x+i][y+i] != origin) {
                    break;
                }
            }
            if (i == n) {
                return origin;
            }
        }
    }

    for (int x = n-1; x < w; ++x) {
        for (int y = 0; y < h - n; ++y) {
            int origin = map[x][y];
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map[x-i][y+i] != origin) {
                    break;
                }
            }
            if (i == n) {
                return origin;
            }
        }
    }

    return 0;
}

bool hasEmpty(const AmobaClient::Map& map) {
    int w = map.size();
    int h = map[0].size();

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            if (map[x][y] == 0) {
                return true;
            }
        }
    }
    return false;
}

}

void AmobaServer::addClient(ClientPtr client) {
    clients.push_back(client);
}

void AmobaServer::run(int xSize, int ySize) {
    AmobaClient::Map map(xSize, std::vector<int>(ySize, 0));

    int clientIndex = 0;
    while (true) {
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
            clients[clientIndex]->getMove(map, clientIndex, clients.size());

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

        map[x][y] = clientIndex;

        clientIndex += 1;
        clientIndex %= clients.size();
    }
}

