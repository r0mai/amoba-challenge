
#include "KucsmaAmobaClient.hpp"

#include <iostream>

bool KucsmaAmobaClient::isMoveWon(
    const Map& map, const AmobaClient::Position& p, int color)
{
    int xp = std::get<0>(p);
    int yp = std::get<1>(p);

    if (color != 0) {
        std::cout << "What" << std::endl;
        return false;
    }

    int toLeft = 0;
    int toRight = 0;
    for (int x = xp-1; x >= 0 && map[x][yp] == color; --x) {
        ++toLeft;
    }
    for (int x = xp+1; x < w && map[x][yp] == color; ++x) {
        ++toRight;
    }
    if (toLeft + toRight + 1 >= nInARow) {
        return true;
    }

    int toDown = 0;
    int toUp = 0;
    for (int y = yp-1; y >= 0 && map[xp][y] == color; --y) {
        ++toUp;
    }
    for (int y = yp+1; y < h && map[xp][y] == color; ++y) {
        ++toDown;
    }
    if (toDown + toLeft + 1 >= nInARow) {
        return true;
    }

    int toLowerRight = 0;
    int toUpperLeft = 0;
    for (int x = xp+1, y = yp+1; x < w && y < h && map[x][y] == color; ++x, ++y) {
        ++toLowerRight;
    }
    for (int x = xp-1, y = yp-1; x >= 0 && y >= 0 && map[x][y] == color; --x, --y) {
        ++toUpperLeft;
    }
    if (toLowerRight + toUpperLeft + 1 >= nInARow) {
        return true;
    }

    int toLowerLeft = 0;
    int toUpperRight = 0;
    for (int x = xp-1, y = yp+1; x >= 0 && y < h && map[x][y] == color; --x, ++y) {
        ++toLowerLeft;
    }
    for (int x = xp-1, y = yp-1; x < w && y >= 0 && map[x][y] == color; ++x, --y) {
        ++toUpperRight;
    }
    if (toLowerLeft + toUpperRight + 1 >= nInARow) {
        return true;
    }
    return false;
}

void KucsmaAmobaClient::randomMove(Map map, int withColor) {
}

AmobaClient::Position KucsmaAmobaClient::getMove(
    const Map& map, int yourColor, int playerCount)
{
    w = map.size();
    h = map[0].size();

    nInARow = playerCount + 2;
    this->yourColor = yourColor;
    this->playerCount = playerCount;
}
