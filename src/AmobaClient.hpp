#pragma once

#include <vector>
#include <tuple>

#include "Map.hpp"

class AmobaClient {
public:

    virtual Position getMove(const Map& map, int yourColor, int playerCount) = 0;
    virtual std::string getName() const = 0;

};
