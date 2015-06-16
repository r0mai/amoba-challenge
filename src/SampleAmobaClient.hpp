#pragma once

#include <string>
#include "AmobaClient.hpp"

class SampleAmobaClient : public AmobaClient {
public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount);
    virtual std::string getName() const;
};
