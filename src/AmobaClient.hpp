#pragma once

#include <vector>
#include <tuple>

class AmobaClient {
public:
    using Position = std::tuple<int, int>;
    using Map = std::vector<std::vector<int>>;

    virtual Position getMove(const Map& map, int yourColor, int playerCount) = 0;
};
