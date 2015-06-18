#pragma once

#include <random>

#include "AmobaClient.hpp"

class KucsmaAmobaClient : public AmobaClient {
public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount);
    virtual std::string getName() const;

private:
    bool isMoveWon(const Map& map, const Position& p, int color);
    std::vector<Position> getNeighbours(const Map& map);

    double runMonteCarlo(const Map& map, Position initialMove);

    int nInARow;
    int w, h;
    int yourColor;
    int playerCount;

    std::random_device rd;
    std::mt19937 generator{rd()};
};
