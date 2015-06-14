#pragma once

#include "AmobaClient.hpp"

class KucsmaAmobaClient : public AmobaClient {
    virtual Position getMove(const Map& map, int yourColor, int playerCount);
};
