#pragma once

#include "AmobaClient.hpp"

class SampleAmobaClient : public AmobaClient {
public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount);
};
