#pragma once

#include "AmobaClient.hpp"

class KucsmaAmobaClient : public AmobaClient {
public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount);

private:
    bool isMoveWon(const Map& map, const AmobaClient::Position& p, int color);
    std::vector<Position> getNeighbours(const Map& map);

    double runMonteCarlo(const Map& map, Position initialMove);

    int nInARow;
    int w, h;
    int yourColor;
    int playerCount;
};
