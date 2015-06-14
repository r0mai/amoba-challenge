#pragma once

#include <vector>
#include <tuple>

class AmobaClient {
public:
    using Position = std::tuple<int, int>;
    using Map = std::vector<std::vector<int>>;

    virtual std::tuple<int, int> getMove(const Map& map, int yourColor) = 0;
};
