#pragma once

#include "AmobaClient.hpp"

class KucsmaAmobaClient : public AmobaClient {
public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount);

private:
    void randomMove(Map map, int withColor);
    bool isMoveWon(const Map& map, const AmobaClient::Position& p, int color);
    std::vector<Position> getNeighbours(const Map& map);

    int nInARow;
    int w, h;
    int yourColor;
    int playerCount;
};
