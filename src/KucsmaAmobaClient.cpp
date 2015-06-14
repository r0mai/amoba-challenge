
#include "KucsmaAmobaClient.hpp"

#include <iostream>

bool KucsmaAmobaClient::isMoveWon(
    const Map& map, const AmobaClient::Position& p, int color)
{
    int xp = std::get<0>(p);
    int yp = std::get<1>(p);

    if (color == 0) {
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
    for (int x = xp+1, y = yp-1; x < w && y >= 0 && map[x][y] == color; ++x, --y) {
        ++toUpperRight;
    }
    if (toLowerLeft + toUpperRight + 1 >= nInARow) {
        return true;
    }
    return false;
}


std::vector<AmobaClient::Position> KucsmaAmobaClient::getNeighbours(
    const Map& map)
{
    std::vector<Position> result;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (map[x][y] != 0) {
                continue;
            }
            Position p(x, y);
            if (x > 0 && map[x-1][y] != 0) { result.push_back(p); continue; }
            if (y > 0 && map[x][y-1] != 0) { result.push_back(p); continue; }
            if (x < w-1 && map[x+1][y] != 0) { result.push_back(p); continue; }
            if (y < h-1 && map[x][y+1] != 0) { result.push_back(p); continue; }

            if (x > 0 && y > 0 && map[x-1][y-1] != 0) { result.push_back(p); continue; }
            if (x > 0 && y < h-1 && map[x-1][y+1] != 0) { result.push_back(p); continue; }
            if (x < w-1 && y > 0 && map[x+1][y-1] != 0) { result.push_back(p); continue; }
            if (x < w-1 && y < h-1 && map[x+1][y+1] != 0) { result.push_back(p); continue; }
        }
    }
    return result;
}

double KucsmaAmobaClient::runMonteCarlo(const Map& origMap, Position initialMove) {

    int x = std::get<0>(initialMove);
    int y = std::get<1>(initialMove);

    int won = 0;
    int lost = 0;
    int draw = 0;

    for (int run = 0; run < 100; ++run) {
        Map map = origMap;
        map[x][y] = yourColor;

        int player = (yourColor + 1) % playerCount + 1;
        bool stop = false;
        while (!stop) {
            auto neighbours = getNeighbours(map);
            if (neighbours.empty()) {
                ++draw;
                break;
            }
            //win if we can
            for (const Position& p : neighbours) {
                if (isMoveWon(map, p, player)) {
                    if (player == yourColor) {
                        ++won;
                        stop = true;
                        break;
                    }
                }
            }
            if (stop) {
                break;
            }
            // block if someone would won
            // TODO only block the next player?
            //
            for (const Position& p : neighbours) {
                if (stop) { break; }
                for (int playeri = 1; playeri <= playerCount; ++playeri) {
                    if (playeri == player) {
                        continue;
                    }
                    if (isMoveWon(map, p, player)) {
                        ++lost;
                        stop = true;
                        break;
                    }
                }
            }
            if (stop) { break; }
            std::uniform_int_distribution<> dis(0, neighbours.size() - 1);
            Position randMove = neighbours[dis(generator)];
            map[std::get<0>(randMove)][std::get<1>(randMove)] = player;

            ++player; if (player == playerCount + 1) player = 1;
        }
    }
    return won - lost;
}

AmobaClient::Position KucsmaAmobaClient::getMove(
    const Map& map, int yourColor, int playerCount)
{
    w = map.size();
    h = map[0].size();

    nInARow = playerCount + 3;
    this->yourColor = yourColor;
    this->playerCount = playerCount;

    auto neighbours = getNeighbours(map);
    if (neighbours.empty()) {
        return Position(w/2, h/2);
    }

    //win if we can
    for (const Position& p : neighbours) {
        if (isMoveWon(map, p, yourColor)) {
            return p;
        }
    }
    // block if someone would won
    // TODO only block the next player?
    for (const Position& p : neighbours) {
        for (int player = 1; player <= playerCount; ++player) {
            if (player == yourColor) {
                continue;
            }
            if (isMoveWon(map, p, player)) {
                return p;
            }
        }
    }

    double max = std::numeric_limits<double>::min();
    Position maxPos;
    for (const Position& p : neighbours) {
        double cur = runMonteCarlo(map, p);
        if (cur > max) {
            max = cur;
            maxPos = p;
        }
    }
    return maxPos;
}
